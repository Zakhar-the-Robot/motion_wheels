
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"

#include "config.h"
#include "macros.h"
#include "position_unit.h"
#include "registers.hpp"
#include "zk_i2c.h"

#if ENABLE_MPU

LOG_SET_TAG("mpu");

#define KFIFOPACKETSIZE 12
static constexpr mpud::gyro_fs_t kGyroFS = mpud::GYRO_FS_500DPS;

// TODO: #9 Pack MPU static variables in an object
static MPU_t MPU; // a default MPU object
static mpud::raw_axes_t accelRaw; // x, y, z axes as int16
static mpud::raw_axes_t gyroRaw; // x, y, z axes as int16
static mpud::raw_axes_t accelBias;
static mpud::raw_axes_t gyroBias;
static mpud::float_axes_t accelG; // accel axes in (g) gravity format
static mpud::float_axes_t gyroDPS; // gyro axes in (DPS) º/s format
static mpud::float_axes_t gyro_calibr; // gyro axes in (DPS) º/s format
static uint32_t mpu_period_ms = 1000 / MPU_SAMPLE_RATE_HZ;
static mpud::float_axes_t angles;
static float angles_threshold = 0.7;

static void update_accel_data(void)
{
    MPU.acceleration(&accelRaw); // fetch raw data from the registers
    accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G); // Convert
}

static void update_gyro_data(void)
{
    MPU.rotation(&gyroRaw); // fetch raw data from the registers
    gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS); // Convert
}

void mpu_centralize_angles(void)
{
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
}

void mpu_reset(void)
{
    mpu_centralize_angles();
}

static void angles_upd(uint32_t ms_from_last_measure, mpud::float_axes_t* old_angles, mpud::float_axes_t* gyro_data)
{
    if (abs(gyro_data->x) > angles_threshold) {
        old_angles->x += gyro_data->x / (1000 / ms_from_last_measure);
    }
    if (abs(gyro_data->y) > angles_threshold) {
        old_angles->y += gyro_data->y / (1000 / ms_from_last_measure);
    }
    if (abs(gyro_data->z) > angles_threshold) {
        old_angles->z += gyro_data->z / (1000 / ms_from_last_measure);
    }
}

void convert_angle_2_sign_and_angle(float val, uint8_t* sign_buf_p, uint8_t* val_buf_p)
{
    uint8_t reg_val;
    bool reg_s;
    int val_int = (int)nearbyint(val);
    reg_s = val >= 0 ? 0 : 1;

    val_int = abs(val_int % 360);

    if (val_int == 0 || val_int == 180) {
        reg_s = false;
        reg_val = val_int;
    } else if (val_int > 180) {
        reg_val = 360 - val_int;
        reg_s = !reg_s;
    } else {
        reg_val = val_int;
    }
    *sign_buf_p = (uint8_t)reg_s;
    *val_buf_p = (uint8_t)reg_val;
}

static void load_angles_to_regs(mpud::float_axes_t* angles)
{
    uint8_t sign = 0xFF;
    uint8_t val = 0xFF;
    convert_angle_2_sign_and_angle(angles->x, &sign, &val);
    SVR_Set(&regs, REG_ANGLE_X_SIGN, sign, false, 0);
    SVR_Set(&regs, REG_ANGLE_X, val, false, 0);
    convert_angle_2_sign_and_angle(angles->y, &sign, &val);
    SVR_Set(&regs, REG_ANGLE_Y_SIGN, sign, false, 0);
    SVR_Set(&regs, REG_ANGLE_Y, val, false, 0);
    convert_angle_2_sign_and_angle(angles->z,&sign, &val);
    SVR_Set(&regs, REG_ANGLE_Z_SIGN, sign, false, 0);
    SVR_Set(&regs, REG_ANGLE_Z, val, false, 0);
}

static void process_gyro_data(void)
{
    angles_upd(mpu_period_ms, &angles, &gyroDPS); // convert (∘/sec) -> (∘)
    load_angles_to_regs(&angles); // load (∘) into uint8_t registers
}

static void mpu_task(void*)
{
    mpu_centralize_angles();
    // LOG_INFO("MPU ready!");
    while (1) {
        update_accel_data();
        update_gyro_data();
        process_gyro_data();
#if PRINT_ANGLES
        printf("angles(prec|int(º)): [ %f\t%f\t%f ]\t", angles[0], angles[1], angles[2]);
        printf("[ %d\t%d\t%d\t%d ]\n", (int8_t)REGR(REG_ANGLE_X), (int8_t)REGR(REG_ANGLE_Y), (uint8_t)REGR(REG_ANGLE_Z_SIGN), (uint8_t)REGR(REG_ANGLE_Z));
#endif
        vTaskDelay(mpu_period_ms / portTICK_PERIOD_MS);
    }
}

esp_err_t start_mpu(void)
{

    ESP_RETURN_ERROR(i2c_master_init(PIN_MPU_I2C_SDA, PIN_MPU_I2C_SCL, MPU_I2C_CLOCK_FREQ_HZ));

    MPU.setBus(i2c0); // set bus port, not really needed since default is i2c0
    MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW); // set address, default is AD0_LOW

    // Verify connection
    while (esp_err_t err = MPU.testConnection()) {
        LOG_ERROR("Failed to connect to the MPU, error=%#X", err);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    LOG_INFO("MPU connection successful!");

    // Initialize
    ESP_RETURN_ERROR(MPU.initialize()); // initialize the chip and set initial configurations

    // Self-Test
    mpud::selftest_t retSelfTest;
    while (esp_err_t err = MPU.selfTest(&retSelfTest)) {
        LOG_ERROR("Failed to perform MPU Self-Test, error=%#X", err);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    LOG_INFO("MPU Self-Test result: Gyro=%s Accel=%s", //
        (retSelfTest & mpud::SELF_TEST_GYRO_FAIL ? "FAIL" : "OK"),
        (retSelfTest & mpud::SELF_TEST_ACCEL_FAIL ? "FAIL" : "OK"));

    // Calibrate
    ESP_RETURN_ERROR(MPU.computeOffsets(&accelBias, &gyroBias));
    ESP_RETURN_ERROR(MPU.setAccelOffset(accelBias));
    ESP_RETURN_ERROR(MPU.setGyroOffset(gyroBias));

    // Configure
    ESP_RETURN_ERROR(MPU.setAccelFullScale(mpud::ACCEL_FS_4G));
    ESP_RETURN_ERROR(MPU.setGyroFullScale(mpud::GYRO_FS_500DPS));
    ESP_RETURN_ERROR(MPU.setSampleRate(MPU_SAMPLE_RATE_HZ));
    ESP_RETURN_ERROR(MPU.setDigitalLowPassFilter(mpud::DLPF_256HZ_NOLPF));

    // Setup FIFO
    ESP_RETURN_ERROR(MPU.setFIFOConfig(mpud::FIFO_CFG_ACCEL | mpud::FIFO_CFG_GYRO));
    ESP_RETURN_ERROR(MPU.setFIFOEnabled(true));

    // Ready to start reading
    ESP_RETURN_ERROR(MPU.resetFIFO()); // start clean

    xTaskCreatePinnedToCore(mpu_task, "mpu_task", 4096, NULL, 5, NULL, 0);
    return ESP_OK;
}

#endif // ENABLE_MPU
