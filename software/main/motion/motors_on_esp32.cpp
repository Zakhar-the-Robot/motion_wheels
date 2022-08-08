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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/mcpwm.h"
#include "esp_log.h"
#include "soc/mcpwm_periph.h"

#include "config.h"
#include "macros.h"
#include "motors_on_esp32.hpp"
#include "port/delay.h"

// LOG_SET_TAG("motors-ll");


MotorsOnEsp32::MotorsOnEsp32(int pin_left_a, int pin_left_b, int pin_right_a, int pin_right_b)
    : pins{ pin_left_a, pin_left_b, pin_right_a, pin_right_b }
{}

void MotorsOnEsp32::Init()
{
    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num     = pins.LeftA,
        .mcpwm0b_out_num     = pins.LeftB,
        .mcpwm1a_out_num     = pins.RightA,
        .mcpwm1b_out_num     = pins.RightB,
        .mcpwm2a_out_num     = -1,  //Not used
        .mcpwm2b_out_num     = -1,  //Not used
        .mcpwm_sync0_in_num  = -1,  //Not used
        .mcpwm_sync1_in_num  = -1,  //Not used
        .mcpwm_sync2_in_num  = -1,  //Not used
        .mcpwm_fault0_in_num = -1,  //Not used
        .mcpwm_fault1_in_num = -1,  //Not used
        .mcpwm_fault2_in_num = -1,  //Not used
        .mcpwm_cap0_in_num   = -1,  //Not used
        .mcpwm_cap1_in_num   = -1,  //Not used
        .mcpwm_cap2_in_num   = -1,  //Not used
    };
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);

    mcpwm_config_t pwm_config;
    pwm_config.frequency    = 25;  //frequency = 25Hz
    pwm_config.cmpr_a       = 50;  //duty cycle of PWMxA = 50.0%
    pwm_config.cmpr_b       = 50;  //duty cycle of PWMxb = 50.0%
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode    = MCPWM_DUTY_MODE_0;

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_LEFT, &pwm_config);
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_RIGHT, &pwm_config);

    set_left_speed(0);
    set_right_speed(0);
}

void MotorsOnEsp32::Stop(uint32_t delay_ms)
{
    gen_duties = { 0, 0, 0, 0 };
    ApplyDuties();
    delay(delay_ms);
}

void MotorsOnEsp32::Forward(float speed, uint32_t delay_ms)
{
    set_left_speed(speed);
    set_right_speed(speed);
    ApplyDuties();
}

void MotorsOnEsp32::Backward(float speed, uint32_t delay_ms)
{
    set_left_speed(speed, MOTOR_REVERSE);
    set_right_speed(speed, MOTOR_REVERSE);
    ApplyDuties();
}

void MotorsOnEsp32::Left(float speed, uint32_t delay_ms)
{
    set_left_speed(speed, MOTOR_REVERSE);
    set_right_speed(speed);
    ApplyDuties();
}

void MotorsOnEsp32::Right(float speed, uint32_t delay_ms)
{
    set_left_speed(speed);
    set_right_speed(speed, MOTOR_REVERSE);
    ApplyDuties();
}

void MotorsOnEsp32::ApplyDuties()
{
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_LEFT, MCPWM_GEN_A, gen_duties.LeftA);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_LEFT, MCPWM_GEN_B, gen_duties.LeftB);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_RIGHT, MCPWM_GEN_A, gen_duties.RightA);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_RIGHT, MCPWM_GEN_B, gen_duties.RightB);
}

void MotorsOnEsp32::set_left_speed(float speed, MotorDirection_t direction)
{
    float duty = speed * MOTORS_SPEED_COEF_L;
    if (direction == MOTOR_NORMAL) {
        gen_duties.LeftA = duty;
        gen_duties.LeftB = 0;
    } else {
        gen_duties.LeftA = 0;
        gen_duties.LeftB = duty;
    }
}

void MotorsOnEsp32::set_right_speed(float speed, MotorDirection_t direction)
{
    float duty = speed * MOTORS_SPEED_COEF_R;
    if (direction == MOTOR_NORMAL) {
        gen_duties.RightA = duty;
        gen_duties.RightB = 0;
    } else {
        gen_duties.RightA = 0;
        gen_duties.RightB = duty;
    }
}

MotorsOnEsp32 motors_esp32(PIN_MOTOR_L1, PIN_MOTOR_L2, PIN_MOTOR_R1, PIN_MOTOR_R2);
