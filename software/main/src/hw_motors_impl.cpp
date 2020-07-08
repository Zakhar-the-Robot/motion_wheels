/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "hw_motors_impl.hpp"
#include "common_config.h"
#include "sdkconfig.h"

static const char *TAG = "motors";

Motors_dc2platform motors(GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25, GPIO_NUM_26);

void init_write_pin(int pin)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE; // disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT; // set as output mode
    io_conf.pin_bit_mask = 1ULL << pin; // bit mask of the pins that you want to set, e.g.GPIO
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE; // disable pull-up mode
    gpio_config(&io_conf); //configure GPIO with the given settings
}


void set_pin(int pin, bool value)
{
    gpio_set_level((gpio_num_t)pin, value);
    ESP_LOGV(TAG, "Pin no.%d set to: %d", pin, value);
}

void motors_delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

void W(void)
{
    motors.MoveForward(0);
}

void S(void)
{
    motors.MoveBackward(0);
}

void A(void)
{
    motors.MoveLeft(0);
}

void D(void)
{
    motors.MoveRight(0);
}

void Shiver(void)
{
    for (size_t i = 0; i < CONFIG_SHIVERS; i++) {
        motors.MoveRight(CONFIG_SHIVER_PERIOD_MS / 2);
        motors.MoveLeft(CONFIG_SHIVER_PERIOD_MS / 2);
    }
    Stop();
}

void Stop(void)
{
    motors.Stop(0);
}