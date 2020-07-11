/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once
#include "driver/gpio.h"
#include "hw_motors.hpp"

extern Motors_dc2platform motors;
void S0(void);
void S1(void);
void S2(void);
void SMAX(void);
void W(void);
void A(void);
void S(void);
void D(void);
void Stop(void);
void Shiver(void);
void start_motors();

typedef enum Speed {
    MOTORS_STOP =       0x00,
    MOTORS_SPEED1 =     0x01,
    MOTORS_SPEED2 =     0x02,
    MOTORS_SPEEDMAX =   0x03,
} MotorsSpeed_t;

