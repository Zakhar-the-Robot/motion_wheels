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

#include "esp_err.h"

esp_err_t init_indication(void);
esp_err_t led_green(void);
esp_err_t led_red(void);
esp_err_t led_off(void);
