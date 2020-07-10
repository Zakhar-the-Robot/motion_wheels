/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#include "common_config.h"

/* Macros
   ********************************************************************* */
#define SIZE_ARR(x) (sizeof(x) / sizeof(x[0]))

#define ESP_RETURN_RES_ON_ERROR(x) do {\
    esp_err_t res = (x);\
    if (res != ESP_OK) return res;\
    } while(0)

/*********************************************************************** Macros*/
/* Registers
   ********************************************************************* */

#define REG_CMD         0x00
#define REG_MODE        0x01
#define REG_LIGHT_LO    0x02
#define REG_LIGHT_HI    0x03
#define REG_GIRO_X      0x04
#define REG_GIRO_Y      0x05
#define REG_GIRO_Z      0x06

/*********************************************************************** Registers*/
/* Commands
   ********************************************************************* */

#define CMD_NONE (-1U)
#define CMD_DONE (0)

#define CMD_FORWARD 119
#define CMD_BACKWARD 115
#define CMD_LEFT 97
#define CMD_RIGHT 100
#define CMD_SHIVER 0x71
#define CMD_STOP_KB 32
#define CMD_STOP 0xA0

#define CMD_TEST 0x74

/*********************************************************************** Commands*/