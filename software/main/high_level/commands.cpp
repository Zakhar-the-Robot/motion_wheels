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

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "commands.hpp"
#include "config.h"
#include "macros.h"
#include "millis.h"
#include "motor_controller.hpp"
#include "position_unit.h"
#include "registers.hpp"

LOG_SET_TAG("cmd");

void S1(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_SetSpeedLow);
}

void S2(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_SetSpeedMedium);

}

void SMAX(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_SetSpeedMax);
}

void W(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Forward);
}

void S(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Backward);
}


void Shiver(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Shiver, MOTORS_SHIVER_DURATION_MS);
}

void Stop(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Stop);
}

#if ENABLE_POSITION_UNIT
static void precise_position(uint8_t current_angle, uint8_t target_angle, bool current_sign)
{
    int delta = abs(current_angle - target_angle);
    ets_printf("Delta: %d\n", delta);
    if (delta > 45) {
        SMAX();
    } else if (delta > 10) {
        S2();
    } else {
        S1();
    }
    if (current_sign) // negative angle
    {
        if (current_angle < target_angle)
            motor_ctrl.Cmd(MotorCmd_Left, 1000);
        if (current_angle > target_angle)
            motor_ctrl.Cmd(MotorCmd_Right, 1000);
    } else { // positive angle
        if (current_angle < target_angle)
            motor_ctrl.Cmd(MotorCmd_Right, 1000);
        if (current_angle > target_angle)
            motor_ctrl.Cmd(MotorCmd_Left, 1000);

    }
}

static void rough_position(MotorControllerCmd_t rot_direction)
{
    SMAX();
    motor_ctrl.Cmd(rot_direction, 5000);
}

static void set_position_do(MotorControllerCmd_t rot_direction, bool target_sign, uint8_t target_angle)
{
    uint8_t current_angle = REGR(REG_ANGLE_Z);
    uint8_t current_sign = REGR(REG_ANGLE_Z_SIGN);
    float old_speed = motor_ctrl.speed;
    uint64_t start_ms = millis();
    while ((current_sign != target_sign) || (current_angle != target_angle)) {
        current_angle = REGR(REG_ANGLE_Z);
        current_sign = REGR(REG_ANGLE_Z_SIGN);
        if (current_sign == target_sign) { // right sign
            precise_position(current_angle, target_angle, current_sign);
        } else { // wait the right sign
            rough_position(rot_direction);
        }
        if(start_ms + MOTORS_POSITIONING_TIMEOUT_MS <= millis()){
            LOG_ERROR("Turning for an angle failed!");
            break; // timeout
        }
        vTaskDelay(1);
    }
    Stop();
    motor_ctrl.speed = old_speed;
}

static void set_position(MotorControllerCmd_t rot_direction, int delta_angle)
{
    uint8_t target_angle;
    uint8_t target_sign;
    if (rot_direction == MotorCmd_Left) {
        delta_angle = -delta_angle;
    }
    convert_angle_2_sign_and_angle(delta_angle, &target_sign, &target_angle);
    mpu_centralize_angles();
    ets_printf("Target: 0x%x:0x%x\n", target_sign, target_angle);
    set_position_do(rot_direction, (bool)target_sign, target_angle);
}
#endif // ENABLE_POSITION_UNIT 

void A(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Left);
#if ENABLE_POSITION_UNIT
    if (regs_get_arg()) {
        set_position(MotorCmd_Left, regs_get_arg());
    }
#endif // ENABLE_POSITION_UNIT
}

void D(void)
{
    LOG_INFO("%s",__func__);
    motor_ctrl.Cmd(MotorCmd_Right);
#if ENABLE_POSITION_UNIT
    if (regs_get_arg()) {
        set_position(MotorCmd_Right, regs_get_arg());
    }
#endif // ENABLE_POSITION_UNIT
}

void set_arg_to_30(void){
    LOG_INFO("%s",__func__);
    REGW(REG_ARG,30);
}
