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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "config.h"
#include "delay.h"
#include "millis.h"
#include "motor_controller.hpp"
#include "motors_on_esp32.hpp"

MotorController motor_ctrl;

MotorController::MotorController()
    : speed(100.0)
{
}

void MotorController::TimeHandler()
{
    if (cmd_state.time_start_ms) {
        uint64_t now_ms = millis();
        uint64_t stop_ms = cmd_state.time_start_ms + cmd_state.durations_ms;
        if (now_ms >= stop_ms) {
            Stop();
        }
    }
}

void MotorController::Stop()
{
    motors_esp32.Stop();
    cmd_state.current_cmd = MotorCmd_None;
    cmd_state.time_start_ms = 0;
    cmd_state.durations_ms = 0;
}

void MotorController::Cmd(MotorControllerCmd_t cmd, uint64_t duration_ms)
{
    float old_speed = this->speed;
    if ((cmd == MotorCmd_Forward) || 
        (cmd == MotorCmd_Backward) || 
        (cmd == MotorCmd_Left) || 
        (cmd == MotorCmd_Right) || 
        (cmd == MotorCmd_Shiver)) {
        this->cmd_state.current_cmd = cmd;
        this->cmd_state.durations_ms = duration_ms;
        this->cmd_state.time_start_ms = millis();
    }
    switch (cmd) {
    case MotorCmd_Stop:
    case MotorCmd_Stop_kb:
        motors_esp32.Stop();
        break;
    case MotorCmd_Forward:
        motors_esp32.Forward(this->speed);
        break;
    case MotorCmd_Backward:
        motors_esp32.Backward(this->speed);
        break;
    case MotorCmd_Left:
        motors_esp32.Left(this->speed);
        break;
    case MotorCmd_Right:
        motors_esp32.Right(this->speed);
        break;
    case MotorCmd_Shiver:
        this->speed = MOTORS_SHIVER_STRENGTH;
        while (this->cmd_state.current_cmd == MotorCmd_Shiver) {
            motors_esp32.Left(this->speed);
            delay(500 / MOTORS_SHIVER_FREQ_HZ);
            motors_esp32.Right(this->speed);
            delay(500 / MOTORS_SHIVER_FREQ_HZ);
        }
        motors_esp32.Stop();
        this->speed = old_speed;
        break;
    case MotorCmd_SetSpeedLow:
        this->speed = MOTORS_SPEED_LOW;
        break;
    case MotorCmd_SetSpeedMedium:
        this->speed = MOTORS_SPEED_MED;
        break;
    case MotorCmd_SetSpeedMax:
        this->speed = MOTORS_SPEED_MAX;
        break;
    default:
        break;
    }
}

static void motor_ctrl_task(void*)
{
    while (1) {
        motor_ctrl.TimeHandler();
        vTaskDelay(1);
    }
}

esp_err_t start_motors(void)
{
    motors_esp32.Init();

#if MOTORS_GREETING

    motors_esp32.Forward(255);
    delay(300*MOTORS_GREETING_LENGTH);
    motors_esp32.Stop(100);

    motors_esp32.Left(120);
    delay(300*MOTORS_GREETING_LENGTH);
    motors_esp32.Stop(100);

    motors_esp32.Right(120);
    delay(300*MOTORS_GREETING_LENGTH);
    motors_esp32.Stop();
#endif //MOTORS_GREETING

#if MOTORS_DEMO // For demo purposes
    motors_esp32.set_left_speed(20, true);
    motors_esp32.set_right_speed(100);
    motors_esp32.ApplyDuties();
#endif //MOTORS_DEMO

    BaseType_t res = xTaskCreatePinnedToCore(&motor_ctrl_task, "motor_ctrl_task", 4096, NULL, 5, NULL, 0);
    return (res == pdTRUE ? ESP_OK : ESP_FAIL);
}
