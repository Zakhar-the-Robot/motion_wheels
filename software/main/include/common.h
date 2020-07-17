// *************************************************************************
//
// Copyright (c) 2020 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#pragma once


/* Peripherals and sw configs
   ********************************************************************* */

#define DEBUG 0
#define PRINT_REGS 0
#define PRINT_ANGLES 0

#define PLATFORM_I2C_ADDRESS 0x2a
#define I2C_COMMAND_DEFINE_PERIOD_MS 1

#define MPU_SAMPLE_RATE_HZ 10
#define MPU_I2C_CLOCK_FREQ_HZ 100000
#define MPU_INIT_CALIBRATION_DELAY_MS 3000

#define MOTORS_SHIVER_PERIOD_MS 100
#define MOTORS_SHIVERS 10

/*********************************************************************** Peripherals and sw configs*/
/* Macros
   ********************************************************************* */
#define SIZE_ARR(x) (sizeof(x) / sizeof(x[0]))

#define ESP_RETURN_RES_ON_ERROR(x) do {\
    esp_err_t res = (x);\
    if (res != ESP_OK) return res;\
    } while(0)

/*********************************************************************** Macros*/
/* Registers
   ********************************************************************* */

enum reristers {
    REG_CMD          = 0x00,
    REG_MODE         = 0x01,
    REG_SPEED        = 0x02,
    REG_RPM_L        = 0x03,
    REG_RPM_R        = 0x04,
    REG_ANGLE_X      = 0x05,
    REG_ANGLE_Y      = 0x06,
    REG_ANGLE_Z      = 0x07,
};

/*********************************************************************** Registers*/
/* Pinout
   ********************************************************************* */
#define PIN_I2C_S_SDA GPIO_NUM_14
#define PIN_I2C_S_SCL GPIO_NUM_27

#define PIN_MPU_I2C_SDA GPIO_NUM_21
#define PIN_MPU_I2C_SCL GPIO_NUM_19

#define PIN_RMP_LEFT GPIO_NUM_22
#define PIN_RMP_RIGHT GPIO_NUM_23

#define PIN_MOTOR_R1 GPIO_NUM_32
#define PIN_MOTOR_R2 GPIO_NUM_33
#define PIN_MOTOR_L1 GPIO_NUM_25
#define PIN_MOTOR_L2 GPIO_NUM_26

/*********************************************************************** Pinout*/
/* Commands
   ********************************************************************* */

#define CMD_NONE (-1U)
#define CMD_DONE (0)

#define CMD_FORWARD 119
#define CMD_BACKWARD 115
#define CMD_LEFT 97
#define CMD_RIGHT 100
#define CMD_SHIVER 0x71
#define CMD_STOP_KB 0x20
#define CMD_STOP 0xA0

#define CMD_SPEED0 0x30
#define CMD_SPEED1 0x31
#define CMD_SPEED2 0x32
#define CMD_SPEED3 0x33

#define CMD_MPU_CALIBRATE 99

#define CMD_TEST 0x74

/*********************************************************************** Commands*/