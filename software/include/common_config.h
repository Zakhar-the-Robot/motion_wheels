// *************************************************************************
//
// Copyright (c) 2020 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once

#define DEBUG 1

#define SERIAL_BAUDRATE 9600

#define MOTORS_STEP_MODE 0
#define MOTORS_STEP_TIMEOUT_MS 35

#define CMD_FORWARD 119
#define CMD_BACKWARD 115
#define CMD_LEFT 97
#define CMD_RIGHT 100
#define CMD_STOP_KB 32
#define CMD_STOP 0xA0

#define PLATFORM_I2C_ADDRESS 0x2a