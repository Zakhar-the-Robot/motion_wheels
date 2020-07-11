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

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "registers.hpp"

static const char *TAG = "serial";

static void serial_task(void *)
{
    // Configure a temporary buffer for the incoming data
    uint8_t ch;
    while (1) {
        ch = fgetc(stdin);
        if (ch != 0xFF) {
            regs.Write(REG_CMD, ch);
            ESP_LOGI(TAG, "Got: %c", regs.Read(REG_CMD));
        }
        vTaskDelay(1);
    }
}

void start_serial(void)
{
    xTaskCreate(&serial_task, "serial_task", 4096, NULL, 5, NULL);
}