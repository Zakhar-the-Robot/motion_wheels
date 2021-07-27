// *************************************************************************
//
// Copyright (c) 2021 Andrei Gramakov. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "config.h"
#include "esp_log.h"
#include "log.h"
#include "motors_on_esp32.hpp"
// Read more https://cpputest.github.io/mocking_manual.html


TEST_GROUP(Motors) {

    void setup() {
        log_set_level(3);
    }

    void teardown()
    {
        mock().clear();
    }

};

TEST(Motors, Basic)
{
    CHECK_EQUAL(motors_esp32.pins.LeftA, PIN_MOTOR_L1);
    CHECK_EQUAL(motors_esp32.pins.LeftB, PIN_MOTOR_L2);
    CHECK_EQUAL(motors_esp32.pins.RightA, PIN_MOTOR_R1);
    CHECK_EQUAL(motors_esp32.pins.RightB, PIN_MOTOR_R2);

    motors_esp32.set_left_speed(100);
    DOUBLES_EQUAL(motors_esp32.gen_duties.LeftA, 100, .01);
    DOUBLES_EQUAL(motors_esp32.gen_duties.LeftB, 0, .01);

    motors_esp32.set_right_speed(74, MOTOR_REVERSE);
    DOUBLES_EQUAL(motors_esp32.gen_duties.RightA, 0, .01);
    DOUBLES_EQUAL(motors_esp32.gen_duties.RightB, 74, .01);

    mock().expectNCalls(4, "mcpwm_set_duty");
    motors_esp32.Stop();
    DOUBLES_EQUAL(motors_esp32.gen_duties.LeftA, 0, .01);
    DOUBLES_EQUAL(motors_esp32.gen_duties.LeftB, 0, .01);
    DOUBLES_EQUAL(motors_esp32.gen_duties.RightA, 0, .01);
    DOUBLES_EQUAL(motors_esp32.gen_duties.RightB, 0, .01);
    mock().checkExpectations();
}
