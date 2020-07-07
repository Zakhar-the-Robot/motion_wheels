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

#include "controlcallback.h"
#include "common_config.h"
#include "hw_motors_impl.hpp"


static Connection_t connection[7] = {
    {.cmd_code = CMD_FORWARD, .func = W},
    {.cmd_code = CMD_BACKWARD, .func = S},
    {.cmd_code = CMD_LEFT, .func = A},
    {.cmd_code = CMD_RIGHT, .func = D},
    {.cmd_code = CMD_SHIVER, .func = Shiver},
    {.cmd_code = CMD_STOP, .func = Stop},
    {.cmd_code = CMD_STOP_KB, .func = Stop}
};

ControlCallbacks Cc(connection, SIZE_ARR(connection));

void control_poll() {
    //TODO replace to regs
    // int new_cmd = i2c.Get(REG_CMD);
    int new_cmd = 0;
    if ((new_cmd != (CMD_NONE&0xFF)) & (new_cmd != CMD_DONE)) {
        //TODO replace to regs
        // i2c.Set(REG_MODE, new_cmd); // exec the cmd
        Cc.Exec(new_cmd);
        //TODO replace to regs
        // i2c.Set(REG_CMD, CMD_DONE); // set that is done
    }
}

