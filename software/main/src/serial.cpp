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

static const char *TAG = "serial";

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE 64

static void serial_task(void *)
{
    // Configure a temporary buffer for the incoming data
    uint8_t ch;
    while (1) {
        ch = fgetc(stdin);
        if (ch != 0xFF) {
            ESP_LOGI(TAG, "Got: %c", ch);
        }
        vTaskDelay(1);
    }
}

void serial_init(void)
{
    xTaskCreate(&serial_task, "serial_task", 4096, NULL, 5, NULL);
}