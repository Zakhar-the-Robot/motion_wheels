/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#include <cstring>
#include "esp_log.h"

#include "zk_i2c.h"
#include "driver/i2c.h"
#include "common.h"
#include "registers.hpp"

static const char *TAG = "zk_i2c";

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

esp_err_t i2c_slave_init(gpio_num_t sda, gpio_num_t scl, uint8_t address)
{
    ESP_LOGI(TAG, "Slave init");
    i2c_config_t conf_slave;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.sda_io_num = sda;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = scl;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = address;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_1, &conf_slave));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_1, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0));
    return ESP_OK;
}

esp_err_t i2c_master_init(gpio_num_t sda, gpio_num_t scl, uint32_t clock)
{
    ESP_LOGI(TAG, "Master init");
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda;
    conf.scl_io_num = scl;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = clock;
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));
    return ESP_OK;
}


static void reg_tx_upd(){
    uint8_t rd_val = regs.GetReg();
    i2c_reset_tx_fifo(I2C_NUM_1);
    i2c_slave_write_buffer(I2C_NUM_1, &rd_val, 1, 0);
}

static void i2c_reader_task(void *)
{
    uint8_t data[3] = {0};
    bool write = false;
    bool read = false;
    int data_count;
    while (1) {
        /* waiting for register to select */
        data_count=0;
        data_count += i2c_slave_read_buffer(I2C_NUM_1, &data[0], 1, portMAX_DELAY);
        data_count += i2c_slave_read_buffer(I2C_NUM_1, &data[1], 1, 0);
        if(data_count != 2)
        {
            ESP_LOGE(TAG, "Wrong frame! Ignore");
            while (1) {
                if (!i2c_slave_read_buffer(I2C_NUM_1, &data[2], 1, 0)) {
                    break;
                }
            }
            continue;
        }
        /* flush others - we are not waiting it */

        if (data[1] != 0xFF) { // write
            regs.Write(data[0], data[1]);
            ESP_LOGI(TAG, "Wrote 0x%x : 0x%x", data[0], data[1]);
        } else { // select
            regs.SelectReg(data[0]);
            reg_tx_upd();
            ESP_LOGI(TAG, "Selected register: 0x%x", data[0]);
        }
    }
}

/**
 * @brief Updates tx fifo with a single byte from the selected register to be
 * read by master
 */
static void i2c_writer_task(void *)
{
    while (1) {
        reg_tx_upd();
        vTaskDelay(1);
    }
}

esp_err_t start_i2c_slave(void)
{
    i2c_slave_init(PIN_I2C_S_SDA, PIN_I2C_S_SCL, PLATFORM_I2C_ADDRESS);
    xTaskCreate(i2c_reader_task, "i2c_reader_task", 1024 * 4, NULL, 5, NULL);
    xTaskCreate(i2c_writer_task, "i2c_writer_task", 1024 * 4, NULL, 6, NULL);
    ESP_LOGI(TAG, "I2C slave ready! Address: 0x%x", PLATFORM_I2C_ADDRESS);
    return ESP_OK;
}
