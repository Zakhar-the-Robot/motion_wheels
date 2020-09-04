/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#pragma once

#include "common.h"
#include "SharedVirtualRegisters.hpp"

#define REGR(n) (regs.Read(n))
#define REGW(n,v) (regs.Write(n,v))

extern SharedVirtualRegisters regs;

inline uint8_t regs_get_cmd(void){
    return regs.Read(REG_CMD);
}

inline uint8_t regs_get_arg(void){
    return regs.Read(REG_ARG);
}

inline uint8_t regs_get_angle_z(void){
    return regs.Read(REG_ANGLE_Z);
}
