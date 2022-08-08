// *************************************************************************
//
// Copyright (c) 2022 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once

// includes
#define CHECK_LOAD_STAGE(func, unit_name)                                                                    \
    do {                                                                                                     \
        esp_err_t res = (func);                                                                              \
        if (res != ESP_OK) {                                                                                 \
            successfull_boot = false;                                                                        \
            LOG_ERROR("Can't initialize %s", unit_name);                                                     \
        }                                                                                                    \
    } while (0)
