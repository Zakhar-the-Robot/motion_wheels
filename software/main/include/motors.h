
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

#include <stdio.h>
#include "esp_err.h"


esp_err_t motors_init(int pin_a1, int pin_a2, int pin_b1, int pin_b2);




#ifdef __cplusplus
}
#endif