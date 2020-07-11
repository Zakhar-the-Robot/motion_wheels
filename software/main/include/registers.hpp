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