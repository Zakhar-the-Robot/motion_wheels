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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/mcpwm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "soc/mcpwm_periph.h"

#include "SharedVirtualRegisters.hpp"

#include "bluetooth_serial.hpp"
#include "config.h"
#include "controlcallback.h"
#include "indication.hpp"
#include "macros.h"
#include "motor_controller.hpp"
#include "position_unit.h"
#include "registers.hpp"
#include "serial.hpp"
#include "zk_i2c.h"

LOG_SET_TAG("main");

#define CHECK_LOAD_STAGE(func, unit_name)                \
    do {                                                 \
        esp_err_t res = (func);                          \
        if (res != ESP_OK) {                             \
            successfull_boot = false;                    \
            LOG_ERROR("Can't initialize %s", unit_name); \
        }                                                \
    } while (0)

static void logging_loop()
{
#if PRINT_REGS
    while (1) {
        vTaskDelay(100 / portTICK_RATE_MS);
        LOG_INFO("regs: [ 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x ]",
            REGR(REG_CMD),
            REGR(REG_ARG),
            REGR(REG_SPEED),
            REGR(REG_ANGLE_X_SIGN),
            REGR(REG_ANGLE_X),
            REGR(REG_ANGLE_Y_SIGN),
            REGR(REG_ANGLE_Y),
            REGR(REG_ANGLE_Z_SIGN),
            REGR(REG_ANGLE_Z));
    }
#endif
}

extern "C" void app_main()
{
    esp_log_level_set("*", ESP_LOG_INFO);
    LOG_INFO("Start!");
    bool successfull_boot = true;
    CHECK_LOAD_STAGE(init_indication(), "Indication");
    led_red(); // boot is started

#if ENABLE_MPU
    /* Each MPU angle is represented as [SIGN : INT_VALUE]
       Values for Z-rotation (parallel to floor):
       [ 0 : 0...180] rotation clockwise for
       [ 1 : 0...179] rotation counter-clockwise
    */
    CHECK_LOAD_STAGE(start_mpu(), "MPU");
#endif // ENABLE_MPU

#if ENABLE_I2C
    CHECK_LOAD_STAGE(start_i2c_slave(), "I2C");
#endif // ENABLE_I2C

    CHECK_LOAD_STAGE(start_motors(), "Motors");
    CHECK_LOAD_STAGE(start_serial(), "Serial control");
    CHECK_LOAD_STAGE(start_control(), "Control system");

#if ENABLE_BT
    CHECK_LOAD_STAGE(start_bt_serial(), "Bluetooth");
#endif // ENABLE_BT

    if (successfull_boot) {
        led_green(); // boot is done with no errors
    }
    LOG_INFO("Init done %s", (successfull_boot ? "successfully" : "with errors"));
    logging_loop();
    while (1) {
        vTaskDelay(1);
    }
}
