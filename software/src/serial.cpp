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

#include "serial.h"
#include "common_config.h"
#include "controlcallback.h"

extern int cmd;

void serial_poll(void)
{
    if (Serial.available() > 0) {
        // read the incoming byte:
        cmd = Serial.read();

        // say what you got:
        printf("[UART] I received: %d\n", cmd);

    }
}

void serial_init(void)
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.print("[UART] Ready! Baudrate: ");
    Serial.println(SERIAL_BAUDRATE);
}