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

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "commands.hpp"
#include "config.h"
#include "controlcallback.h"
#include "macros.h"
#include "position_unit.h"
#include "registers.hpp"

LOG_SET_TAG("ctrl");

static Connection_t connection[] = {
    {.cmd_code = CMD_FORWARD,           .func = W               },
    {.cmd_code = CMD_BACKWARD,          .func = S               },
    {.cmd_code = CMD_LEFT,              .func = A               },
    {.cmd_code = CMD_RIGHT,             .func = D               },
    {.cmd_code = CMD_SHIVER,            .func = Shiver          },
    {.cmd_code = CMD_STOP,              .func = Stop            },
    {.cmd_code = CMD_STOP_KB,           .func = Stop            },
    {.cmd_code = CMD_SPEED1,            .func = S1              },
    {.cmd_code = CMD_SPEED2,            .func = S2              },
    {.cmd_code = CMD_SPEED3,            .func = SMAX            },
    {.cmd_code = CMD_SET_ARG_TO_30,     .func = set_arg_to_30   },
#if ENABLE_MPU
    {.cmd_code = CMD_MPU_CALIBRATE,     .func = mpu_reset       },
#endif // ENABLE_MPU

};

ControlCallbacks Cc(connection, SIZE_ARR(connection));

void control_poll(void *)
{
    while (1) {
        int new_cmd = REGR(REG_CMD);
        if ((new_cmd != (CMD_NONE & 0xFF)) & (new_cmd != CMD_DONE)) {
            LOG_DEBUG("New Command: 0x%x", new_cmd);
            Cc.Exec(new_cmd);
            REGW(REG_CMD,CMD_DONE);
            if (new_cmd != CMD_SET_ARG_TO_30){
                REGW(REG_ARG,0);
            }
        }
        vTaskDelay(1);
    }
}

esp_err_t start_control(void)
{
    BaseType_t res = xTaskCreatePinnedToCore(&control_poll, "control_poll", 4096, NULL, 5, NULL, 1);
    return (res == pdTRUE ? ESP_OK : ESP_FAIL);
}
