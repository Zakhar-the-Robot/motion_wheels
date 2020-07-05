/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#include "motors.hpp"
#include "driver/gpio.h"
#include "hw_motors.hpp"

static const char* TAG = "motors";

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