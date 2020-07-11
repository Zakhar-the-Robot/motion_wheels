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

#include "common.h"
#include "controlcallback.h"
#include "hw_motors_impl.hpp"
#include "registers.hpp"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ControlCallbacks";

static Connection_t connection[] = {
    {.cmd_code = CMD_FORWARD,   .func = W       },
    {.cmd_code = CMD_BACKWARD,  .func = S       },
    {.cmd_code = CMD_LEFT,      .func = A       },
    {.cmd_code = CMD_RIGHT,     .func = D       },
    {.cmd_code = CMD_SHIVER,    .func = Shiver  },
    {.cmd_code = CMD_STOP,      .func = Stop    },
    {.cmd_code = CMD_STOP_KB,   .func = Stop    },
    {.cmd_code = CMD_SPEED0,    .func = S0      },
    {.cmd_code = CMD_SPEED1,    .func = S1      },
    {.cmd_code = CMD_SPEED2,    .func = S2      },
    {.cmd_code = CMD_SPEED3,    .func = SMAX    },
};

ControlCallbacks Cc(connection, SIZE_ARR(connection));

void control_poll(void *)
{
    while (1) {
        // int new_cmd = i2c.Get(REG_CMD);
        int new_cmd = regs.Read(REG_CMD);
        if ((new_cmd != (CMD_NONE & 0xFF)) & (new_cmd != CMD_DONE)) {
            ESP_LOGD(TAG, "New Command: 0x%x", new_cmd);
            Cc.Exec(new_cmd);
            regs.Write(REG_CMD, CMD_DONE);
        }
        vTaskDelay(1);
    }
}

void start_control(void)
{
    xTaskCreate(&control_poll, "control_poll", 4096, NULL, 5, NULL);
}