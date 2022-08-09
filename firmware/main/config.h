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

#include "hal/gpio_types.h"
// clang-format off

/* Config
   ********************************************************************* */
#define DEBUG 1
#define PRINT_REGS 0
#define PRINT_ANGLES 0

#define ENABLE_BLUETOOTH_SERIAL 0
#define ENABLE_I2C 0
#define ENABLE_POSITION_UNIT 1
#define ENABLE_CAN 1

#define BT_DEVICE_NAME "Zakhar: Wheels"
#define BT_SPP_SERVER_NAME "SPP_SERVER"
#define BT_SPP_SHOW_DATA 0
#define BT_SPP_SHOW_SPEED 1
#define BT_SPP_SHOW_MODE SPP_SHOW_DATA    /*Choose show mode: show data or speed*/

#define CAN_ADDRESS                         0x2
#define CAN_ID_MASTER_STOP_CMD              0x0A0
#define CAN_ID_MASTER_START_CMD             0x0A1
#define CAN_ID_MASTER_PING                  0x0A2
#define CAN_ID_SLAVE_STOP_RESP              0x0B0
#define CAN_ID_SLAVE_DATA                   0x0B1
#define CAN_ID_SLAVE_PING_RESP              0x0B2

#define I2C_COMMAND_DEFINE_PERIOD_MS 1
#define I2C_DATA_LENGTH 512 /*!< Data buffer length of test buffer */
#define I2C_SLAVE_ADDRESS 0x2a
#define I2C_SLAVE_RX_BUF_LEN (2 * I2C_DATA_LENGTH) /*!< I2C slave rx buffer size */
#define I2C_SLAVE_TX_BUF_LEN (2 * I2C_DATA_LENGTH) /*!< I2C slave tx buffer size */

#define MPU_I2C_CLOCK_FREQ_HZ 100000
#define MPU_INIT_CALIBRATION_DELAY_MS 3000
#define MPU_SAMPLE_RATE_HZ 100

#define MOTORS_DEMO                   0
#define MOTORS_GREETING               1
#define MOTORS_GREETING_LENGTH        2.0f
#define MOTORS_POSITIONING_TIMEOUT_MS 3000U
#define MOTORS_SHIVER_DURATION_MS     1000U
#define MOTORS_SHIVER_FREQ_HZ         6U
#define MOTORS_SHIVER_STRENGTH        100.0f
#define MOTORS_SPEED_LOW              45.0f
#define MOTORS_SPEED_MAX              100.0f
#define MOTORS_SPEED_MED              65.0f
#define MOTORS_SPEED_COEF_L           1.0f
#define MOTORS_SPEED_COEF_R           1.0f

/* Pinout
   ********************************************************************* */
#define PIN_MPU_I2C_SDA GPIO_NUM_5
#define PIN_MPU_I2C_SCL GPIO_NUM_18

#define PIN_CANBUS_TX_ON_MODULE   GPIO_NUM_25 // WARM/LIGHT COLOR and GREEN
#define PIN_CANBUS_RX_ON_MODULE   GPIO_NUM_26 // COLD/DARK COLOR (e.g. BLUE)

#define PIN_GREEN GPIO_NUM_16
#define PIN_RED GPIO_NUM_17

#define PIN_MOTOR_L1 GPIO_NUM_12
#define PIN_MOTOR_L2 GPIO_NUM_13
#define PIN_MOTOR_R1 GPIO_NUM_14
#define PIN_MOTOR_R2 GPIO_NUM_27
