
/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

void init_write_pin(int pin);
void set_pin(int pin, bool value);

#ifdef __cplusplus
}
#endif