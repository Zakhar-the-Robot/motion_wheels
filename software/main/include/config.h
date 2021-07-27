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

#include "hal/gpio_types.h"

/* Config
   ********************************************************************* */

#define DEBUG 1
#define PRINT_REGS 0
#define PRINT_ANGLES 0

#define ENABLE_BT 1
#define ENABLE_I2C 1
#define ENABLE_MPU 1

#define BT_DEVICE_NAME "Zakhar the robot"
#define BT_SPP_SERVER_NAME "SPP_SERVER"
#define BT_SPP_SHOW_DATA 0
#define BT_SPP_SHOW_SPEED 1
#define BT_SPP_SHOW_MODE SPP_SHOW_DATA    /*Choose show mode: show data or speed*/

#define I2C_COMMAND_DEFINE_PERIOD_MS 1
#define I2C_DATA_LENGTH 512 /*!< Data buffer length of test buffer */
#define I2C_SLAVE_ADDRESS 0x2a
#define I2C_SLAVE_RX_BUF_LEN (2 * I2C_DATA_LENGTH) /*!< I2C slave rx buffer size */
#define I2C_SLAVE_TX_BUF_LEN (2 * I2C_DATA_LENGTH) /*!< I2C slave tx buffer size */

#define MPU_I2C_CLOCK_FREQ_HZ 100000
#define MPU_INIT_CALIBRATION_DELAY_MS 3000
#define MPU_SAMPLE_RATE_HZ 100

#define MOTORS_DEMO 0
#define MOTORS_GREETING 1
#define MOTORS_POSITIONING_TIMEOUT_MS 3000U
#define MOTORS_SHIVER_DURATION_MS 1000U
#define MOTORS_SHIVER_FREQ_HZ 6U
#define MOTORS_SHIVER_STRENGTH 100.0f
#define MOTORS_SPEED_LOW 45.0f
#define MOTORS_SPEED_MAX 100.0f
#define MOTORS_SPEED_MED 65.0f
#define MOTORS_SPEED_COEF_L 1.0f
#define MOTORS_SPEED_COEF_R 1.0f

/* Pinout
   ********************************************************************* */
#define PIN_I2C_S_SDA GPIO_NUM_18
#define PIN_I2C_S_SCL GPIO_NUM_33

#define PIN_MPU_I2C_SDA GPIO_NUM_27
#define PIN_MPU_I2C_SCL GPIO_NUM_25

#define PIN_GREEN GPIO_NUM_21
#define PIN_RED GPIO_NUM_22

#define PIN_MOTOR_R1 GPIO_NUM_5
#define PIN_MOTOR_R2 GPIO_NUM_26
#define PIN_MOTOR_L1 GPIO_NUM_23
#define PIN_MOTOR_L2 GPIO_NUM_19
