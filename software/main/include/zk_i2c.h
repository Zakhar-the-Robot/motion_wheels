/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "common.h"

esp_err_t i2c_master_init(gpio_num_t sda, gpio_num_t scl, uint32_t clock);
esp_err_t i2c_slave_init(gpio_num_t sda, gpio_num_t scl, uint8_t address);
esp_err_t start_i2c_slave(void);