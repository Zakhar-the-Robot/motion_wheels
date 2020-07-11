/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "common.h"
#include "rotsensors.hpp"
#include "registers.hpp"
static const char *TAG = "RPMs";

// TODO: pack
static int rpm_l = 0;
static int rpm_r = 0;
static xQueueHandle gpio_evt_queue = NULL;


static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void rmp_gpio_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // check
    io_conf.pin_bit_mask = (1 << PIN_RMP_LEFT) | (1 << PIN_RMP_RIGHT);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(30, sizeof(uint32_t));
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PIN_RMP_LEFT, gpio_isr_handler, (void *) PIN_RMP_LEFT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(PIN_RMP_RIGHT, gpio_isr_handler, (void *) PIN_RMP_RIGHT);
}

static void rpms_task(void *)
{
    int period_ms = 500;
    int r_per_min_l = 0;
    int r_per_min_r = 0;
    int disk_gaps = 24;
    while (1) {
        r_per_min_l = (60000 * rpm_l) / (period_ms * disk_gaps); // TODO check the calculation!
        r_per_min_r = (60000 * rpm_r) / (period_ms * disk_gaps);
        REGW(REG_RPM_L, r_per_min_l);
        REGW(REG_RPM_R, r_per_min_r);
        ESP_LOGD(TAG, "RPM_L: %d, RPM_R: %d", REGR(REG_RPM_L), REGR(REG_RPM_R));
        rpm_l = 0;
        rpm_r = 0;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void rpms_gpio_task(void *)
{
    uint32_t io_num;
    while(1) {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == PIN_RMP_LEFT) {
                rpm_l++;
            }
            if (io_num == PIN_RMP_RIGHT) {
                rpm_r++;
            }
        }
    }
}

void start_rpms()
{
    rmp_gpio_init();
    xTaskCreate(rpms_task, "rpms_task", 4096, NULL, 5, NULL);
    xTaskCreate(rpms_gpio_task, "rpms_task", 4096, NULL, 5, NULL);
}