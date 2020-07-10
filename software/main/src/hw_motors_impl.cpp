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
#include "common_config.h"
#include "sdkconfig.h"

static const char *TAG = "motors";

static  uint32_t stop_ms = 0;
static  uint32_t go_ms = 0;
TimerHandle_t timer;
static  uint32_t timer_ms = 0;
static bool stopped = true;

typedef  void (Motors_dc2platform::*Motors_dc2platform_pnt_t)(uint32_t);

static  Motors_dc2platform_pnt_t action = NULL;


Motors_dc2platform motors(GPIO_NUM_32, GPIO_NUM_33, GPIO_NUM_25, GPIO_NUM_26);

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

void W(void)
{
    timer_start(100);
    action = &Motors_dc2platform::MoveForward ;
    stop_ms = 0;
    go_ms = 100;
}

void S(void)
{
    motors.MoveForward(500);
}

void A(void)
{
    timer_start(100);
    action = &Motors_dc2platform::MoveForward ;
    stop_ms = 1;
    go_ms = 10;
}

void D(void)
{
    timer_start(100);
    action = &Motors_dc2platform::MoveForward ;
    stop_ms = 5;
    go_ms = 10;
}

#define TEST_PER_MS 15
void Test(void)
{
    motors.MoveForward(0);
    motors_delay(30);
    for (size_t i = 0; i < (500 / TEST_PER_MS); i++) {
        motors.MoveForward(TEST_PER_MS);
        motors.Stop(20);
    }
}

void Shiver(void)
{
    for (size_t i = 0; i < CONFIG_SHIVERS; i++) {
        motors.MoveRight(CONFIG_SHIVER_PERIOD_MS / 2);
        motors.MoveLeft(CONFIG_SHIVER_PERIOD_MS / 2);
    }
    Stop();
}

void Stop(void)
{
    timer_ms = 0;
    xTimerStop(timer,0);
    motors.Stop(150);
}


// class MotorsWithSpeed : private Motors_dc2platform {
// public:
//     void BackwardForward(MotorsSpeed_t speed, uint32_t ms);
//     void LeftRight(MotorsSpeed_t speed, uint32_t ms);
//     void Stop(uint32_t ms);
// private:
//     bool stopped;
// };

// MotorsWithSpeed::BackwardForward(MotorsSpeed_t speed, uint32_t ms)
// {
//     uint32_t zero_ms, one_ms;
//     switch (speed) {
//     case MOTORS_SPEEDm3:
//         break;
//     case MOTORS_SPEEDm2:
//         break;
//     case MOTORS_SPEEDm1:
//         break;
//     case MOTORS_STOP:
//         this->Stop(ms);
//         break;
//     case MOTORS_SPEED1:
//         break;
//     case MOTORS_SPEED2:
//         break;
//     case MOTORS_SPEED3:
//         break;
//     default:
//         break;
//     }
// }


static void motors_task(void *)
{
    ESP_LOGI(TAG, "Task Started!");
    while (1) {
        if (xTimerIsTimerActive(timer)) {
            if (action) {
                stopped = false;
                (motors.*action)(0);
                motors_delay(go_ms);
                if (stop_ms) {
                    motors.Stop(0);
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

void vTimerCallback( TimerHandle_t xTimer ) {xTimerStop(timer,0);};



void start_motors()
{
    motors.MoveForward(100);
    motors.Stop(150);
    motors.MoveLeft(150);
    motors.Stop(150);
    motors.MoveRight(150);
    timer = xTimerCreate ( "Timer", 1, pdTRUE, ( void * ) 0, vTimerCallback );
    xTaskCreate(motors_task, "motors_task", 4096, NULL, 6, NULL);
}