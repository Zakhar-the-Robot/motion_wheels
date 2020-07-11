/* Copyright (c) 2020 Andrei Gramakov. All rights reserved.
 *
 * This file is licensed under the terms of the MIT license.
 * For a copy, see: https://opensource.org/licenses/MIT
 *
 * site:    https://agramakov.me
 * e-mail:  mail@agramakov.me
 */

#include "esp_log.h"

#include "zk_i2c.h"
#include "driver/i2c.h"

static const char* TAG = "zk_i2c";

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


esp_err_t start_i2c_slave(void)
{
    uint8_t addr = 0x2a;
    i2c_slave_init(GPIO_NUM_13, GPIO_NUM_12, addr);
    ESP_LOGI(TAG, "I2C slave ready! Address: 0x%x", addr);
    return ESP_OK;
}
