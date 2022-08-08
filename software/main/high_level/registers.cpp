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

#include "registers.hpp"

SharedVirtualRegisters_t regs;


int RegistersInit(){
    SVR_Init(&regs, REGS_AMOUNT);
    return 0;
}
