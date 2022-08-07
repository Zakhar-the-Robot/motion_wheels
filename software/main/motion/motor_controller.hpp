// *************************************************************************
//
// Copyright (c) 2021 Andrei Gramakov. All rights reserved.
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

#include "esp_err.h"
#include "esp_log.h"

#include "commands.hpp"
#include "config.h"

typedef enum {
    MotorCmd_None = 0x0,
    MotorCmd_Stop = CMD_STOP,
    MotorCmd_Stop_kb = CMD_STOP_KB,
    MotorCmd_Forward = CMD_FORWARD,
    MotorCmd_Backward = CMD_BACKWARD,
    MotorCmd_Left = CMD_LEFT,
    MotorCmd_Right = CMD_RIGHT,
    MotorCmd_Shiver = CMD_SHIVER,
    MotorCmd_SetSpeedLow = CMD_SPEED1,
    MotorCmd_SetSpeedMedium = CMD_SPEED2,
    MotorCmd_SetSpeedMax = CMD_SPEED3
} MotorControllerCmd_t;

typedef struct {
    MotorControllerCmd_t current_cmd;
    uint64_t time_start_ms;
    uint64_t durations_ms;
} MotorControllerCmdState_t;

class MotorController {
private:
    MotorControllerCmdState_t cmd_state;

public:
    float speed;
    MotorController();
    void TimeHandler();
    void Cmd(MotorControllerCmd_t cmd, uint64_t duration_ms = 300);
    void Stop();
};

extern MotorController motor_ctrl;
esp_err_t start_motors(void);
