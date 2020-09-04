/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once
#include "esp_err.h"
#include "driver/gpio.h"
#include "hw_motors.hpp"


typedef void (Motors_dc2platform::*Motors_dc2platform_pnt_t)(uint32_t);

extern Motors_dc2platform motors;
esp_err_t start_motors();

typedef enum Speed {
    MOTORS_STOP =       0x00,
    MOTORS_SPEED1 =     0x01,
    MOTORS_SPEED2 =     0x02,
    MOTORS_SPEEDMAX =   0x03,
} MotorsSpeed_t;

void SetSpeed(MotorsSpeed_t speed);
void SetAction(Motors_dc2platform_pnt_t a);
void SetStopped(bool state);
bool IsStopped(void);
esp_err_t timer_start_ms(size_t ms);
esp_err_t timer_start(void);
esp_err_t timer_stop(void);