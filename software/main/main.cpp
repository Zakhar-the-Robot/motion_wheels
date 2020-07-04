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
#include "position_unit.h"

static const char* TAG = "Main task";

// https://www.freertos.org/FreeRTOS_Support_Forum_Archive/November_2013/freertos_FreeRTOS_C_and_xTaskCreate_function_da582460j.html

extern "C" void app_main() {
    ESP_LOGI(TAG, "Start!");

    i2c_master_init(GPIO_NUM_22, GPIO_NUM_23, 100000);
    mpu_init();
    ESP_LOGI(TAG, "Init done");

}
