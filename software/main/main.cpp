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
#include "rotsensors.hpp"

static const char *TAG = "Main task";


extern "C" void app_main()
{
    ESP_LOGI(TAG, "Start!");
    start_mpu();
    start_i2c_slave();
    start_motors();
    start_serial();
    start_control();
    start_rpms();
    ESP_LOGI(TAG, "Init done");


}
