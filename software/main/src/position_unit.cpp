
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

#include "common.h"
#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "zk_i2c.h"
#include "registers.hpp"

#include "position_unit.h"

static const char *TAG = "pos";


// TODO: pack
static MPU_t MPU;  // a default MPU object
static mpud::raw_axes_t accelRaw;   // x, y, z axes as int16
static mpud::raw_axes_t gyroRaw;    // x, y, z axes as int16
static mpud::float_axes_t accelG;   // accel axes in (g) gravity format
static mpud::float_axes_t gyroDPS;  // gyro axes in (DPS) º/s format
static mpud::float_axes_t gyro_calibr;  // gyro axes in (DPS) º/s format
static uint32_t mpu_period_ms = 1000 / MPU_SAMPLE_RATE_HZ;
static mpud::float_axes_t angles;
static float angles_threshold = 0.7;

void mpu_centralize_angles(void)
{
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
}

void mpu_calibrate(void)
{
    ESP_LOGI(TAG, "MPU calibration. Don't move anything!");
    vTaskDelay(100 / portTICK_PERIOD_MS);
    MPU.rotation(&gyroRaw);       // fetch raw data from the registers
    gyro_calibr = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);
    ESP_LOGI(TAG, "MPU calibrated!");
}

void mpu_reset(void)
{
    mpu_calibrate();
    mpu_centralize_angles();
}

static void angles_upd(uint32_t ms_from_last_measure, mpud::float_axes_t *old_angles, mpud::float_axes_t *gyro_data)
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

static void apply_calibr_data(mpud::float_axes_t *data, mpud::float_axes_t *calibr_diff)
{
    data->x -= calibr_diff->x;
    data->y -= calibr_diff->y;
    data->z -= calibr_diff->z;
}

static void mpu_task(void *)
{
    MPU.rotation(&gyroRaw);       // fetch raw data from the registers
    vTaskDelay(MPU_INIT_CALIBRATION_DELAY_MS / portTICK_PERIOD_MS);
    mpu_calibrate();
    mpu_centralize_angles();
    ESP_LOGI(TAG, "MPU ready!");
    while (1) {
        // Read
        MPU.acceleration(&accelRaw);  // fetch raw data from the registers
        MPU.rotation(&gyroRaw);       // fetch raw data from the registers
        // MPU.motion(&accelRaw, &gyroRaw);  // read both in one shot
        // Convert
        accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G);
        gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);
        apply_calibr_data(&gyroDPS, &gyro_calibr);
        // Debug
        // printf("accel: [%+6.2f %+6.2f %+6.2f ] (G) \t", accelG.x, accelG.y, accelG.z);
        // printf("gyro: [%+7.2f %+7.2f %+7.2f ] (º/s)\t", gyroDPS[0], gyroDPS[1], gyroDPS[2]);

        angles_upd(mpu_period_ms, &angles, &gyroDPS);
        REGW(REG_ANGLE_X,angles[0]);
        REGW(REG_ANGLE_Y,angles[1]);
        REGW(REG_ANGLE_Z,angles[2]);
    #if PRINT_ANGLES
        printf("angles: [ %d\t%d\t%d ] (º)\n", (int8_t)REGR(REG_ANGLE_X), (int8_t)REGR(REG_ANGLE_Y), (int8_t)REGR(REG_ANGLE_Z));
    #endif
        vTaskDelay(mpu_period_ms / portTICK_PERIOD_MS);
    }
}

esp_err_t start_mpu(void)
{

    ESP_RETURN_RES_ON_ERROR(i2c_master_init(PIN_MPU_I2C_SDA, PIN_MPU_I2C_SCL, MPU_I2C_CLOCK_FREQ_HZ));

    MPU.setBus(i2c0);  // set bus port, not really needed since default is i2c0
    MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);  // set address, default is AD0_LOW

    esp_err_t res = MPU.testConnection();
    if (res != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to the MPU, error=%#X", res);
        return res;
    }
    ESP_LOGI(TAG, "MPU connection successful!");

    // Initialize
    ESP_ERROR_CHECK(MPU.initialize());  // initialize the chip and set initial configurations
    // Setup with your configurations
    ESP_ERROR_CHECK(MPU.setSampleRate(MPU_SAMPLE_RATE_HZ));
    // ESP_ERROR_CHECK(MPU.setGyroFullScale(mpud::GYRO_FS_500DPS));
    // ESP_ERROR_CHECK(MPU.setAccelFullScale(mpud::ACCEL_FS_4G));

    xTaskCreate(mpu_task, "mpu_task", 4096, NULL, 5, NULL);
    return ESP_OK;
}
