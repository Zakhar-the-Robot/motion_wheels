// *************************************************************************
//
// Copyright (c) 2022 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once

#include <stdint.h>

#include "driver/mcpwm.h"
#include "esp_err.h"

#define MCPWM_TIMER_LEFT MCPWM_TIMER_0
#define MCPWM_TIMER_RIGHT MCPWM_TIMER_1

typedef enum {
    MOTOR_NORMAL = 0x0,
    MOTOR_REVERSE,
} MotorDirection_t;

class MotorsOnEsp32 {
public:
    struct {
        int LeftA;
        int LeftB;
        int RightA;
        int RightB;
    } pins;

    struct {
        float LeftA;
        float LeftB;
        float RightA;
        float RightB;
    } gen_duties;

    MotorsOnEsp32(int pin_left_a, int pin_left_b, int pin_right_a, int pin_right_b);
    void Init();
    void Forward(float speed, uint32_t delay_ms = 0);
    void Backward(float speed, uint32_t delay_ms = 0);
    void Left(float speed, uint32_t delay_ms = 0);
    void Right(float speed, uint32_t delay_ms = 0);
    void Stop(uint32_t delay_ms = 0);
    void set_left_speed(float speed, MotorDirection_t direction = MOTOR_NORMAL);
    void set_right_speed(float speed, MotorDirection_t direction = MOTOR_NORMAL);
    void ApplyDuties();

protected:
private:
};

extern MotorsOnEsp32 motors_esp32;
esp_err_t start_motors(void);
