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

#pragma once

#include "esp_err.h"

#define SIZE_ARR(x) (sizeof(x) / sizeof(x[0]))

#define ESP_RETURN_ERROR(x)  \
    do {                     \
        esp_err_t res = (x); \
        if (res != ESP_OK)   \
            return res;      \
    } while (0)

#define ESTR(x) (esp_err_to_name(x))

#define LOG_SET_TAG(tag) static const char* TAG = tag

#define LOG_ERROR(format, ...) ESP_LOGE(TAG, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) ESP_LOGW(TAG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) ESP_LOGI(TAG, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) ESP_LOGD(TAG, format, ##__VA_ARGS__)
#define LOG_VERBOSE(format, ...) ESP_LOGV(TAG, format, ##__VA_ARGS__)
