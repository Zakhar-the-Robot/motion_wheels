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

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"
#include <stdint.h>

esp_err_t start_mpu(void);
void mpu_centralize_angles(void);
void mpu_reset(void);
void convert_angle_2_sign_and_angle(float val, uint8_t* reg_s_p, uint8_t* reg_val_p);

#ifdef __cplusplus
}
#endif
