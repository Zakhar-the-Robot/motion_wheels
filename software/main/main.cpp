// =========================================================================
// Released under the MIT License
// Copyright 2017-2018 Natanael Josue Rabello. All rights reserved.
// For the license information refer to LICENSE file in root directory.
// =========================================================================

/**
 * @file mpu_i2c.cpp
 * Example on how to setup MPU through I2C for basic usage.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "sdkconfig.h"

#include "common.h"
#include "zk_i2c.h"
#include "hw_motors_impl.hpp"
#include "position_unit.h"
#include "serial.h"
#include "controlcallback.h"
#include "SharedVirtualRegisters.hpp"
#include "bluetooth_serial.hpp"
#include "rotsensors.hpp"
#include "registers.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"

static const char *TAG = "main";

// TODO implement i2c registers select and read/write
extern "C" void app_main()
{
    ESP_LOGI(TAG, "Start!");
    start_mpu();
    start_i2c_slave();
    start_motors();
    start_serial();
    start_control();
    start_rpms();
    start_bt_serial();
    ESP_LOGI(TAG, "Init done");

#if PRINT_REGS
    while (1) {
        vTaskDelay(100 / portTICK_RATE_MS);
        ESP_LOGI(TAG, "regs: [ 0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x\t0x%x ]",
                 REGR(REG_CMD),
                 REGR(REG_MODE),
                 REGR(REG_SPEED),
                 REGR(REG_RPM_L),
                 REGR(REG_RPM_R),
                 REGR(REG_ANGLE_X),
                 REGR(REG_ANGLE_Y),
                 REGR(REG_ANGLE_Z)
                );
    }
#endif

}
