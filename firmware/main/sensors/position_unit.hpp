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

#include <stdint.h>
#include "esp_err.h"

esp_err_t start_mpu(void);
void      mpu_centralize_angles(void);
void      mpu_reset(void);
void      convert_angle_2_sign_and_angle(float val, uint8_t *sign_buf_p, uint8_t *val_buf_p);

#ifdef __cplusplus
}
#endif
