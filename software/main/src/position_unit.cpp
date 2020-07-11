
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

#include "position_unit.h"

static const char *TAG = "pos";

static MPU_t MPU;  // a default MPU object

static mpud::raw_axes_t accelRaw;   // x, y, z axes as int16
static mpud::raw_axes_t gyroRaw;    // x, y, z axes as int16
static mpud::float_axes_t accelG;   // accel axes in (g) gravity format
static mpud::float_axes_t gyroDPS;  // gyro axes in (DPS) º/s format

static void mpu_task(void *)
{
    ESP_LOGI(TAG, "MPU ready!");
    while (1) {
        // Read
        MPU.acceleration(&accelRaw);  // fetch raw data from the registers
        MPU.rotation(&gyroRaw);       // fetch raw data from the registers
        // MPU.motion(&accelRaw, &gyroRaw);  // read both in one shot
        // Convert
        accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G);
        gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);
        // Debug
        printf("accel: [%+6.2f %+6.2f %+6.2f ] (G) \t", accelG.x, accelG.y, accelG.z);
        printf("gyro: [%+7.2f %+7.2f %+7.2f ] (º/s)\n", gyroDPS[0], gyroDPS[1], gyroDPS[2]);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

esp_err_t start_mpu(void)
{

    ESP_RETURN_RES_ON_ERROR(i2c_master_init(GPIO_NUM_21, GPIO_NUM_19, 100000));

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
    // ESP_ERROR_CHECK(MPU.setSampleRate(50));  // set sample rate to 50 Hz
    // ESP_ERROR_CHECK(MPU.setGyroFullScale(mpud::GYRO_FS_500DPS));
    // ESP_ERROR_CHECK(MPU.setAccelFullScale(mpud::ACCEL_FS_4G));

    xTaskCreate(mpu_task, "mpu_task", 4096, NULL, 5, NULL);
    return ESP_OK;
}
