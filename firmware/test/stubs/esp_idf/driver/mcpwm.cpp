// *************************************************************************
//
// Copyright (c) 2020 Andrei Gramakov. All rights reserved.
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
#include "mcpwm.h"

esp_err_t mcpwm_init( mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, const mcpwm_config_t  *mcpwm_conf){
    return mock().actualCall(__FUNCTION__)
//          .withParameterOfType("mcpwm_unit_t","mcpwm_num", &mcpwm_num)
//          .withParameterOfType("mcpwm_timer_t", "timer_num", &timer_num)
//          .withParameterOfType("mcpwm_config_t", "mcpwm_conf", mcpwm_conf)
          .returnIntValue();
}

esp_err_t mcpwm_set_pin(mcpwm_unit_t mcpwm_num, const mcpwm_pin_config_t *mcpwm_pin){
    return mock().actualCall(__FUNCTION__)
//                 .withParameterOfType("mcpwm_unit_t", "mcpwm_num", &mcpwm_num)
//                 .withParameterOfType("mcpwm_pin_config_t", "mcpwm_pin", mcpwm_pin)
                 .returnIntValue();
}

esp_err_t mcpwm_set_duty(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_generator_t gen, float duty){
    return mock().actualCall(__FUNCTION__)
//        .withParameterOfType("mcpwm_unit_t", "mcpwm_num", &mcpwm_num)
//        .withParameterOfType("mcpwm_timer_t", "timer_num", &timer_num)
//        .withParameterOfType("mcpwm_generator_t", "gen", &gen)
//        .withParameterOfType("float", "duty", &duty)
        .returnIntValue();
}
