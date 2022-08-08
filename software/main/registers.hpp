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

#include "SharedVirtualRegisters.h"
#include "config.h"

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

extern SharedVirtualRegisters_t regs;

inline uint8_t REGR(uint8_t n)
{
    SVR_reg_t res = 0xFF;
    SVR_Get(&regs, n, &res, false, 0);
    return (uint8_t) res;
}

inline void REGW(uint8_t n, uint8_t v) { SVR_Set(&regs, n, v, false, 0); }

inline uint8_t regs_get_cmd(void)
{
    SVR_reg_t res = 0xFF;
    SVR_Get(&regs, REG_CMD, &res, false, 0);
    return (uint8_t) res;
}

inline uint8_t regs_get_arg(void)
{
    SVR_reg_t res = 0xFF;
    SVR_Get(&regs, REG_ARG, &res, false, 0);
    return (uint8_t) res;
}

inline uint8_t regs_get_angle_z(void)
{
    SVR_reg_t res = 0xFF;
    SVR_Get(&regs, REG_ANGLE_Z, &res, false, 0);
    return (uint8_t) res;
}

int RegistersInit();
