/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#include "SharedVirtualRegisters.hpp"

#define REG_CMD         0x00
#define REG_MODE        0x01
#define REG_LIGHT_LO    0x02
#define REG_LIGHT_HI    0x03
#define REG_GIRO_X      0x04
#define REG_GIRO_Y      0x05
#define REG_GIRO_Z      0x06

extern SharedVirtualRegisters regs;