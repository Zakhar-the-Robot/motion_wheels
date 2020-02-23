/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#include <Arduino.h>
#include "ControlCallbacks.h"
#include "hw_motors.h"


#define SIZE_ARR(x) (sizeof(x) / sizeof(x[0]))


#define PINR1 9
#define PINR2 10
#define PINL1 11
#define PINL2 12

void motors_greeting(void);
void W(void);
void A(void);
void S(void);
void D(void);
void Stop(void);

// Connection_t connection[5];
// Motors_dc2platform wheels;