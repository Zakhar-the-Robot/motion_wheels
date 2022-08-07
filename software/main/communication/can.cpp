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

#include <stdio.h>
#include <stdlib.h>

#include "can.hpp"
#include "canbus.hpp"
#include "config.h"
#include "driver/twai.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "registers.hpp"

#define TAG "CAN"

void CmdCallback(CanBus* dev, twai_message_t& rMsg)
{
    ESP_LOGW(TAG, "RxCallback! msgid: 0x%x", rMsg.identifier);
    ESP_LOGD(TAG, "Command!");
    ESP_LOGD(TAG, "Data for me!");
    ESP_LOGD(TAG,
        "Data: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x,",
        rMsg.data[0], rMsg.data[1], rMsg.data[2], rMsg.data[3],
        rMsg.data[4], rMsg.data[5], rMsg.data[6], rMsg.data[7]);
    REGW(REG_CMD, rMsg.data[0]);
    REGW(REG_ARG, rMsg.data[1]);
}

esp_err_t start_can()
{

    ESP_LOGI(TAG, "CAN start... (dev:0x%x)", (uint32_t)&devCanBus);
    devCanBus.Start(CAN_ADDRESS,
        PIN_CANBUS_TX_ON_MODULE,
        PIN_CANBUS_RX_ON_MODULE);

    ESP_LOGI(TAG, "Setting up the Store on receiving...");
    devCanBus.SetCallbackRxCmd(CmdCallback);

    return ESP_OK;
}
