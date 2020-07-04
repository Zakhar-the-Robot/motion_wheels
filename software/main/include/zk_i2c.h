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
#include "driver/gpio.h"
#include "esp_err.h"

#define I2C_DEFAULT_SDA_MASTER_PIN GPIO_NUM_22
#define I2C_DEFAULT_SCL_MASTER_PIN GPIO_NUM_23
#define I2C_DEFAULT_MASTER_CLOCK 100000

#define I2C_DEFAULT_SDA_SLAVE_PIN GPIO_NUM_18
#define I2C_DEFAULT_SCL_SLAVE_PIN GPIO_NUM_19

esp_err_t i2c_master_init(gpio_num_t sda, gpio_num_t scl, uint32_t clock);
esp_err_t i2c_slave_init(gpio_num_t sda, gpio_num_t scl);