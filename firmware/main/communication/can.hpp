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


#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

extern uint8_t can_data_storage[8];
esp_err_t      start_can();

#ifdef __cplusplus
}
#endif
