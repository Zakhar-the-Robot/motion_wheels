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
#include "hw_motors.hpp"
#include "position_unit.h"

static const char *TAG = "Main task";

// https://www.freertos.org/FreeRTOS_Support_Forum_Archive/November_2013/freertos_FreeRTOS_C_and_xTaskCreate_function_da582460j.html

#define ESP_RETURN_RES_ON_ERROR(x) do {\
    esp_err_t res = (x);\
    if (res != ESP_OK) return res;\
    } while(0)


void init_write_pin(int pin)
{
    gpio_config_t io_conf;

    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;

    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;

    // bit mask of the pins that you want to set, e.g.GPIO
    io_conf.pin_bit_mask = 1ULL << pin;

    // disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;

    // disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    //configure GPIO with the given settings
    gpio_config(&io_conf);
}


void set_pin(int pin, bool value)
{
    gpio_set_level((gpio_num_t)pin, value);
    ESP_LOGI(TAG, "Pin no.%d set to: %d", pin, value);
}

static esp_err_t start_mpu(void)
{
    ESP_RETURN_RES_ON_ERROR(i2c_master_init(GPIO_NUM_22, GPIO_NUM_23, 100000));
    ESP_RETURN_RES_ON_ERROR(mpu_init());
    return ESP_OK;
}

static Motors_dc2platform motors(GPIO_NUM_26, GPIO_NUM_25, GPIO_NUM_33, GPIO_NUM_32);

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Start!");
    start_mpu();

    while (1) {
        motors.MoveForward();
        vTaskDelay(1000 / portTICK_RATE_MS);
        motors.MoveBackward();
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    ESP_LOGI(TAG, "Init done");

}
