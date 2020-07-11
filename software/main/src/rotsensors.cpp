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
static const char *TAG = "RPMs";

// TODO: pack
static int rpm_l = 0;
static int rpm_r = 0;
static xQueueHandle gpio_evt_queue = NULL;


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void rmp_gpio_init(void){
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // check
    io_conf.pin_bit_mask = (1<<GPIO_NUM_22) | (1<<GPIO_NUM_23);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(30, sizeof(uint32_t));
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_22, gpio_isr_handler, (void*) GPIO_NUM_22);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_23, gpio_isr_handler, (void*) GPIO_NUM_23);
}


static void rpms_task(void*){
    int period_ms = 500;
    int r_per_min_l = 0;
    int r_per_min_r = 0;
    int disk_gaps = 24;
    while(1){
        r_per_min_l = (60000 * rpm_l) / (period_ms * disk_gaps); // TODO check the calculation!
        r_per_min_r = (60000 * rpm_r) / (period_ms * disk_gaps);
        ESP_LOGD(TAG, "RPM_L: %d, RPM_R: %d", r_per_min_l, r_per_min_r);
        rpm_l = 0;
        rpm_r = 0;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void rpms_gpio_task(void*){
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            if (io_num == GPIO_NUM_22)
                rpm_l++;
            if (io_num == GPIO_NUM_23)
                rpm_r++;
            // printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level((gpio_num_t)io_num));
        }
    }
}

void start_rpms(){
    rmp_gpio_init();
    xTaskCreate(rpms_task, "rpms_task", 4096, NULL, 5, NULL);
    xTaskCreate(rpms_gpio_task, "rpms_task", 4096, NULL, 5, NULL);
}