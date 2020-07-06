// =========================================================================
// Released under the MIT License
// Copyright 2017-2018 Natanael Josue Rabello. All rights reserved.
// For the license information refer to LICENSE file in root directory.
// =========================================================================

/**
 * @file mpu_i2c.cpp
 * Example on how to setup MPU through I2C for basic usage.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "sdkconfig.h"

#include "zk_i2c.h"
#include "hw_motors_impl.hpp"
#include "position_unit.h"

static const char *TAG = "Main task";

#define ESP_RETURN_RES_ON_ERROR(x) do {\
    esp_err_t res = (x);\
    if (res != ESP_OK) return res;\
    } while(0)


static esp_err_t start_mpu(void)
{
    ESP_RETURN_RES_ON_ERROR(i2c_master_init(GPIO_NUM_22, GPIO_NUM_23, 100000));
    ESP_RETURN_RES_ON_ERROR(mpu_init());
    ESP_LOGI(TAG, "MPU ready!");
    return ESP_OK;
}

static esp_err_t start_i2c_slave(void)
{
    uint8_t addr = 0x2a;
    i2c_slave_init(GPIO_NUM_19, GPIO_NUM_21, addr);
    ESP_LOGI(TAG, "I2C slave ready! Address: 0x%x", addr);
    return ESP_OK;
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Start!");
    // start_mpu();
    start_i2c_slave();
    while (1) {
        motors.MoveForward();
        vTaskDelay(1000 / portTICK_RATE_MS);
        motors.MoveBackward();
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    ESP_LOGI(TAG, "Init done");

}
