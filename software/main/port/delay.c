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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "delay.h"

void delay(unsigned ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}
