/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "hw_motors_impl.hpp"
#include "common.h"
#include "sdkconfig.h"

static const char *TAG = "motors";

typedef  void (Motors_dc2platform::*Motors_dc2platform_pnt_t)(uint32_t);


// TODO: pack
static  uint32_t stop_ms = 0;
static  uint32_t go_ms = 0;
TimerHandle_t timer;
static  uint32_t timer_ms = 0;
static MotorsSpeed_t current_speed = MOTORS_STOP;
static bool stopped = true;
static  Motors_dc2platform_pnt_t action = NULL;
Motors_dc2platform motors(PIN_MOTOR_R1, PIN_MOTOR_R2, PIN_MOTOR_L1, PIN_MOTOR_L2);


static esp_err_t timer_start(size_t ms)
{

    if ( xTimerChangePeriod( timer, (ms / portTICK_PERIOD_MS), 0 ) != pdPASS ) {
        return ESP_FAIL;
    }
    if ( xTimerStart( timer, 0 ) != pdPASS ) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

void init_write_pin(int pin)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE; // disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT; // set as output mode
    io_conf.pin_bit_mask = 1ULL << pin; // bit mask of the pins that you want to set, e.g.GPIO
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE; // disable pull-up mode
    gpio_config(&io_conf); //configure GPIO with the given settings
}


void set_pin(int pin, bool value)
{
    gpio_set_level((gpio_num_t)pin, value);
    ESP_LOGV(TAG, "Pin no.%d set to: %d", pin, value);
}

void motors_delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

static void SetSpeed(MotorsSpeed_t speed)
{
    switch (speed) {
    case MOTORS_SPEED1:
        go_ms = 10;
        stop_ms = 10;
        break;
    case MOTORS_SPEED2:
        go_ms = 20;
        stop_ms = 10;
        break;
    case MOTORS_SPEEDMAX:
        go_ms = 50;
        stop_ms = 0;
        break;
    case MOTORS_STOP:
        action = NULL;
        go_ms = 0;
        stop_ms = 0;
        break;
    default:
        break;
    }
    current_speed = speed;
}



void S0(void)
{
    SetSpeed(MOTORS_STOP);
}

void S1(void)
{
    SetSpeed(MOTORS_SPEED1);
}

void S2(void)
{
    SetSpeed(MOTORS_SPEED2);
}

void SMAX(void)
{
    SetSpeed(MOTORS_SPEEDMAX);
}

void W(void)
{
    stopped = false;
    timer_start(100);
    action = &Motors_dc2platform::MoveForward;
}

void S(void)
{
    stopped = false;
    timer_start(100);
    action = &Motors_dc2platform::MoveBackward;
}

void A(void)
{

    stopped = false;
    timer_start(100);
    action = &Motors_dc2platform::MoveLeft;

}

void D(void)
{
    stopped = false;
    timer_start(100);
    action = &Motors_dc2platform::MoveRight;
}


void Shiver(void)
{
    for (size_t i = 0; i < MOTORS_SHIVERS; i++) {
        motors.MoveRight(MOTORS_SHIVER_PERIOD_MS / 2);
        motors.MoveLeft(MOTORS_SHIVER_PERIOD_MS / 2);
    }
    Stop();
}

void Stop(void)
{
    stopped = true;
    timer_ms = 0;
    xTimerStop(timer, 0);
}


static void motors_task(void *)
{
    ESP_LOGI(TAG, "Task Started!");
    while (1) {
        if (xTimerIsTimerActive(timer)) {
            if (action) {
                if (go_ms) {
                    (motors.*action)(go_ms);
                }
                if (stop_ms) {
                    motors_delay(stop_ms);
                }
            }
        } else { // time is up - full stop
            if (action) {
                action = NULL;
            }
            if (!stopped) {
                motors.Stop(0);
                stopped = true;
            }
        }
        vTaskDelay(1);
    }

}

void vTimerCallback( TimerHandle_t xTimer )
{
    xTimerStop(timer, 0);
};

void start_motors()
{
    motors.MoveForward(100);
    motors.Stop(150);
    motors.MoveLeft(150);
    motors.Stop(150);
    motors.MoveRight(150);
    timer = xTimerCreate ( "Timer", 1, pdTRUE, ( void * ) 0, vTimerCallback );
    xTaskCreate(motors_task, "motors_task", 4096, NULL, 7, NULL);
}