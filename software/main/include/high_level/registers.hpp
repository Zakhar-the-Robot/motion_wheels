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

#include "SharedVirtualRegisters.hpp"
#include "config.h"

#define REGR(n) (regs.Read(n))
#define REGW(n,v) (regs.Write(n,v))

enum registers {
    REG_CMD = 0x00,
    REG_ARG,
    REG_MODE,
    REG_SPEED,
    REG_ANGLE_X_SIGN,
    REG_ANGLE_X,
    REG_ANGLE_Y_SIGN,
    REG_ANGLE_Y,
    REG_ANGLE_Z_SIGN,
    REG_ANGLE_Z,
    REGS_AMOUNT
};

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
