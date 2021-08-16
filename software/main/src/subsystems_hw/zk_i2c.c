// *************************************************************************
//
// Copyright (c) 2021 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/i2c.h"
#include "esp_log.h"
#include "hal/i2c_hal.h"
#include "soc/i2c_periph.h"

#include "config.h"
#include "hal/i2c_ll.h"
#include "macros.h"
#include "registers.hpp"
#include "soc/i2c_struct.h"
#include "zk_i2c.h"
#include "driver/periph_ctrl.h"


LOG_SET_TAG("zk_i2c");

#define I2C_CLEAR_MASK 0x1FFF
#define I2C_ENTER_CRITICAL_ISR(mux)    portENTER_CRITICAL_ISR(mux)
#define I2C_EXIT_CRITICAL_ISR(mux)     portEXIT_CRITICAL_ISR(mux)
#define I2C_ENTER_CRITICAL(mux)        portENTER_CRITICAL(mux)
#define I2C_EXIT_CRITICAL(mux)         portEXIT_CRITICAL(mux)

static intr_handle_t intr_handle; /*!< I2C interrupt handle*/
static uint8_t zk_i2c_reg;
static uint8_t zk_i2c_reg_val_rcv;
static uint8_t zk_i2c_reg_val_to_send;
static uint8_t zk_i2c_start_count;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
typedef enum {
    ZK_I2C_STATE_IDLE = 0x00,
    ZK_I2C_STATE_STARTED,
    ZK_I2C_STATE_GOT_REGISTER,
    ZK_I2C_STATE_GOT_REGISTER_AND_DATA,
    ZK_I2C_STATE_REQUESTED_DATA,
    ZK_I2C_STATE_STOPED,
} zk_i2c_status_t;
static zk_i2c_status_t zk_i2c_status;

static void zk_i2c_reset()
{
    i2c_dev_t* i2c_dev = I2C_LL_GET_HW(I2C_NUM_1);
    zk_i2c_status = ZK_I2C_STATE_IDLE;
    zk_i2c_reg = 0xFF;
    zk_i2c_reg_val_rcv = 0xFF;
    zk_i2c_reg_val_to_send = 0xFF;
    i2c_dev->int_clr.val = I2C_CLEAR_MASK;
    i2c_ll_txfifo_rst(i2c_dev);
    i2c_ll_rxfifo_rst(i2c_dev);
}

static void i2c_hw_enable(i2c_port_t i2c_num)
{
    I2C_ENTER_CRITICAL(&spinlock);
    periph_module_enable(i2c_periph_signal[i2c_num].module);
    I2C_EXIT_CRITICAL(&spinlock);
}
// TODO: #7 rewrite the handler
static void IRAM_ATTR zk_i2c_isr_handler(void* arg)
{
    // static void IRAM_ATTR i2c_isr_handler_default(void *arg)
    // i2c_obj_t *p_i2c = (i2c_obj_t *) arg;
    // int i2c_num = p_i2c->i2c_num;
    // i2c_intr_event_t evt_type = I2C_INTR_EVENT_ERR;
    portBASE_TYPE HPTaskAwoken = pdFALSE;
    i2c_dev_t* i2c_dev = I2C_LL_GET_HW(I2C_NUM_1);
    ets_printf("I2c ISR begin 0x%08x\n\r", i2c_dev->int_status.val); 
    
    if (i2c_dev->int_status.trans_start) {
        ets_printf("I2c ISR trans_start\n\r"); 
        // zk_i2c_start_count += 1;
        i2c_dev->int_clr.trans_start = 1;

    }
    // if (i2c_dev->int_status.rx_fifo_full || zk_i2c_start_count) {
    //     uint32_t rx_fifo_cnt = i2c_ll_get_rxfifo_cnt(i2c_dev);
    //     if (zk_i2c_start_count == 1 || rx_fifo_cnt >= 2) { // write to this device
    //         i2c_ll_read_rxfifo(i2c_dev, &zk_i2c_reg, 1);
    //         i2c_ll_read_rxfifo(i2c_dev, &zk_i2c_reg_val_rcv, 1);
    //     } else if (zk_i2c_start_count == 1 || rx_fifo_cnt == 1) { // got register
    //         i2c_ll_read_rxfifo(i2c_dev, &zk_i2c_reg, 1);
    //         SVR_Get(&regs, zk_i2c_reg, &zk_i2c_reg_val_to_send, true, 1000);
    //         i2c_ll_write_txfifo(i2c_dev, &zk_i2c_reg_val_to_send, 1);
    //     }
    // }
    // // if (zk_i2c_start_count == 1 || i2c_dev->int_status.rx_fifo_full) {
    // //     uint32_t rx_fifo_cnt = i2c_ll_get_rxfifo_cnt(i2c_dev);
    // //     i2c_ll_read_rxfifo(i2c_dev, &zk_i2c_reg, 1);
    // // }
    // // if (zk_i2c_start_count == 2 || i2c_dev->int_status.rx_fifo_full) {
    // //     uint32_t rx_fifo_cnt = i2c_ll_get_rxfifo_cnt(i2c_dev);
    // //     i2c_ll_read_rxfifo(i2c_dev, &zk_i2c_reg, 1);
    // // }

    // if (i2c_dev->int_status.end_detect) {
    //     zk_i2c_start_count = 0;
    //     zk_i2c_reset();
    //     i2c_dev->int_clr.end_detect = 1;
    // }
    if (i2c_dev->int_status.ack_err){
        ets_printf("I2c ISR ack_err\n\r"); 
        i2c_dev->int_clr.ack_err = 1;
    }

    if (i2c_dev->int_status.arbitration_lost){
        ets_printf("I2c ISR arbitration_lost\n\r"); 
        i2c_dev->int_clr.arbitration_lost = 1;
    }

    if (i2c_dev->int_raw.rx_fifo_full){
        ets_printf("I2c ISR rx_fifo_full\n\r"); 
        i2c_dev->int_clr.rx_fifo_full = 1;
        i2c_ll_rxfifo_rst(i2c_dev);
    }

    if (i2c_dev->int_raw.trans_complete) {
        // ets_printf("I2c ISR trans_complete\n\r");
        ets_printf("I2c ISR trans_complete (rx_fifo_cnt: 0x%04x)\n\r", i2c_dev->status_reg.rx_fifo_cnt);
        if (i2c_dev->int_raw.rx_rec_full) {
            ets_printf("I2c ISR rx_rec_full (rx_fifo_cnt: 0x%04x)\n\r", i2c_dev->status_reg.rx_fifo_cnt);

            zk_i2c_reg = i2c_dev->fifo_data.data;
            ets_printf("I2C zk_i2c_reg 0x%04x\n\r", zk_i2c_reg);
            i2c_dev->fifo_data.data = zk_i2c_reg;

            if (i2c_dev->status_reg.rx_fifo_cnt == 1) {
                zk_i2c_reg_val_rcv = i2c_dev->fifo_data.data;
                ets_printf("I2C zk_i2c_reg_val_rcv 0x%04x\n\r", zk_i2c_reg_val_rcv);
            } else if (!i2c_dev->status_reg.rx_fifo_cnt) {
                ets_printf("I2C read! Put data to tx here\n\r");
            } else {
                ets_printf("[error] I2C fifo extra bytes! %d\n\r", i2c_dev->status_reg.rx_fifo_cnt);
                i2c_ll_rxfifo_rst(i2c_dev);
            }
            i2c_dev->int_clr.rx_rec_full = 1;
        }

        i2c_dev->int_clr.trans_complete = 1;
        // zk_i2c_reset();
    }

    if (i2c_dev->int_raw.slave_tran_comp){
        // ets_printf("I2c ISR slave_tran_comp\n\r"); 
        ets_printf("I2c ISR slave_tran_comp (rx_fifo_cnt: 0x%04x)\n\r", i2c_dev->status_reg.rx_fifo_cnt); 
        if (i2c_dev->int_raw.rx_rec_full) {
            ets_printf("I2c ISR rx_rec_full (rx_fifo_cnt: 0x%04x)\n\r", i2c_dev->status_reg.rx_fifo_cnt);
            
            zk_i2c_reg = i2c_dev->fifo_data.data;
            ets_printf("I2C zk_i2c_reg 0x%04x\n\r", zk_i2c_reg);
            ets_printf("I2c ISR rx_rec_full (rx_fifo_cnt: 0x%04x)\n\r", i2c_dev->status_reg.rx_fifo_cnt);

            i2c_dev->fifo_data.data = zk_i2c_reg;
            if (i2c_dev->status_reg.rx_fifo_cnt){
                zk_i2c_reg_val_rcv = i2c_dev->fifo_data.data;
                ets_printf("I2C zk_i2c_reg_val_rcv 0x%04x\n\r", zk_i2c_reg_val_rcv);
            } else {
                uint32_t i = rand();
                i2c_dev->fifo_data.data = i;
                ets_printf("I2C send 0x%04x\n\r", i);
                // i2c_ll_rxfifo_rst(i2c_dev);
            }

            // if (i2c_dev->status_reg.rx_fifo_cnt == 1) {
            //     zk_i2c_reg_val_rcv = i2c_dev->fifo_data.data;
            //     ets_printf("I2C zk_i2c_reg_val_rcv 0x%04x\n\r", zk_i2c_reg_val_rcv);
            // } else if (!i2c_dev->status_reg.rx_fifo_cnt) {
            //     ets_printf("I2C read! Put data to tx here\n\r");
            // } else {
            //     ets_printf("[error] I2C fifo extra bytes! %d\n\r", i2c_dev->status_reg.rx_fifo_cnt);
            //     // i2c_ll_rxfifo_rst(i2c_dev);
            // }
            i2c_dev->int_clr.rx_rec_full = 1;
        }
        i2c_dev->int_clr.slave_tran_comp = 1;
        // zk_i2c_reset();
    }
    // ets_printf("I2C fifo 0x%08x\n\r", i2c_dev->fifo_data.data);
    // ets_printf("I2C fifo 0x%08x\n\r", i2c_dev->fifo_data.data);
    // ets_printf("I2C fifo 0x%08x\n\r", i2c_dev->fifo_data.data);
    // ets_printf("I2C fifo 0x%08x\n\r", i2c_dev->fifo_data.data);
    // } else {
    //     if (i2c_dev->int_status.trans_start |)
    // }

    // // if (i2c_dev->int_status.end_detect) {
    // //     uint32_t rx_fifo_cnt = i2c_ll_get_rxfifo_cnt(i2c_dev);
    //     //     i2c_hal_read_rxfifo(&(i2c_context[i2c_num].hal), p_i2c->data_buf, rx_fifo_cnt);
    //     //     xRingbufferSendFromISR(p_i2c->rx_ring_buf, p_i2c->data_buf, rx_fifo_cnt, &HPTaskAwoken);
    //     //     i2c_hal_slave_clr_rx_it(&(i2c_context[i2c_num].hal));
    // // } else if (evt_type == I2C_INTR_EVENT_TXFIFO_EMPTY) {
    // //     uint32_t tx_fifo_rem = i2c_ll_get_txfifo_len(i2c_dev);
    //     //     size_t size = 0;
    //     //     uint8_t *data = (uint8_t *) xRingbufferReceiveUpToFromISR(p_i2c->tx_ring_buf, &size, tx_fifo_rem);
    //     //     if (data) {
    //     //         i2c_hal_write_txfifo(&(i2c_context[i2c_num].hal), data, size);
    //     //         vRingbufferReturnItemFromISR(p_i2c->tx_ring_buf, data, &HPTaskAwoken);
    //     //     } else {
    //     //         i2c_hal_disable_slave_tx_it(&(i2c_context[i2c_num].hal));
    //     //     }
    //     //     i2c_hal_slave_clr_tx_it(&(i2c_context[i2c_num].hal));
    // // }
    //We only need to check here if there is a high-priority task needs to be switched.
    ets_printf("I2c ISR end 0x%08x\n\r\n\r", i2c_dev->int_status.val); 

    if (HPTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

#if ENABLE_MPU
esp_err_t i2c_master_init(gpio_num_t sda, gpio_num_t scl, uint32_t clock)
{
    LOG_INFO("Master init");
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = sda;
    conf.scl_io_num = scl;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = clock;
    conf.clk_flags = 0;
    ESP_RETURN_ERROR(i2c_param_config(I2C_NUM_0, &conf));
    ESP_RETURN_ERROR(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));
    return ESP_OK;
}
#endif // ENABLE_MPU

#if ENABLE_I2C

esp_err_t i2c_slave_init(gpio_num_t sda, gpio_num_t scl, uint8_t address)
{
    LOG_INFO("Slave init");
    i2c_config_t conf_slave;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.sda_io_num = sda;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = scl;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = address;
    ESP_RETURN_ERROR(i2c_param_config(I2C_NUM_1, &conf_slave));
    // ESP_RETURN_ERROR(i2c_driver_install(I2C_NUM_1, conf_slave.mode,
    //     I2C_SLAVE_RX_BUF_LEN,
    //     I2C_SLAVE_TX_BUF_LEN, 0));


    i2c_dev_t* i2c_dev = ((((1)) == 0) ? &I2C0 : &I2C1);

    i2c_hw_enable(I2C_NUM_1);

    //Disable I2C interrupt.
    i2c_ll_disable_intr_mask(i2c_dev, I2C_LL_INTR_MASK);
    i2c_ll_clr_intsts_mask(i2c_dev, I2C_LL_INTR_MASK);
    i2c_isr_register(I2C_NUM_1, zk_i2c_isr_handler, NULL, 0, &intr_handle);
    i2c_dev->fifo_conf.nonfifo_rx_thres = 1;
    // i2c_dev->scl_start_hold.time = 0x1;
    // i2c_dev->scl_rstart_setup.time = 0x1;
    // i2c_ll_slave_enable_rx_it(i2c_dev);
    // Enable interruptions:
    // i2c_dev->int_ena.val = I2C_TRANS_COMPLETE_INT_ENA_M | 
    //                         I2C_TRANS_START_INT_ENA_M | 
    //                         I2C_RXFIFO_FULL_INT_ENA_M |
    //                         I2C_RX_REC_FULL_INT_ST_M;

    i2c_dev->int_ena.val = 0;
    i2c_dev->int_ena.ack_err = 1;
    // i2c_dev->int_ena.arbitration_lost = 1;
    // i2c_dev->int_ena.end_detect = 1;
    // i2c_dev->int_ena.master_tran_comp = 1;
    i2c_dev->int_ena.rx_fifo_full = 1;
    i2c_dev->int_ena.rx_fifo_ovf = 1;
    i2c_dev->int_ena.rx_rec_full = 1;
    i2c_dev->int_ena.slave_tran_comp = 1;
    i2c_dev->int_ena.time_out = 1;
    // i2c_dev->int_ena.trans_complete = 1;
    i2c_dev->int_ena.trans_start = 1;
    // i2c_dev->int_ena.tx_fifo_empty = 1;
    // i2c_dev->int_ena.tx_send_empty = 1;

    // i2c_ll_slave_enable_tx_it(i2c_dev);
    // Init all to the initial state
    // zk_i2c_reset();

    return ESP_OK;
}

// /**/**
//  * @brief
//  *
//  */
//  *
//  * @return esp_err_t
//  */
// static esp_err_t reg_tx_upd()
// {
//     uint8_t rd_val = regs.GetReg();
//     ESP_RETURN_ERROR(i2c_reset_tx_fifo(I2C_NUM_1));
//     int res = i2c_slave_write_buffer(I2C_NUM_1, &rd_val, 1, 0);
//     if (!res) {
//         return ESP_FAIL;
//     }
//     return ESP_OK;
// }

// static void i2c_reader_task(void*)
// {
//     uint8_t data[3] = { 0 };
//     int data_count;
//     while (1) {
//         /* waiting for register to select */
//         data_count = 0;
//         data_count += i2c_slave_read_buffer(I2C_NUM_1, &data[0], 1, portMAX_DELAY);
//         data_count += i2c_slave_read_buffer(I2C_NUM_1, &data[1], 1, 0);
//         if (data_count != 2) {
//             LOG_ERROR("Wrong frame! Ignore");
//             while (1) {
//                 if (!i2c_slave_read_buffer(I2C_NUM_1, &data[2], 1, 0)) {
//                     break;
//                 }
//             }
//             continue;
//         }
//         /* flush others - we are not waiting it */
//         if (data[1] != 0xFF) {
//             // write
//             REGW(data[0], data[1]);
//             LOG_INFO("Wrote 0x%x : 0x%x", data[0], data[1]);
//         } else {
//             // select
//             regs.SelectReg(data[0]);
//             esp_err_t res = reg_tx_upd();
//             if (res != ESP_OK) {
//                 LOG_ERROR("Cannot send a value (%s)", ESTR(res));
//             }
//             LOG_INFO("Selected register: 0x%x", data[0]);
//         }
//     }
// }

/**
 * @brief Updates tx fifo with a single byte from the selected register to be
 * read by master
 */
// static void i2c_writer_task(void*)
// {
//     esp_err_t res = 0;
//     while (1) {
//         res = reg_tx_upd();
//         if (res != ESP_OK) {
//             LOG_ERROR("Can't load a value to the TX FIFO!");
//         }
//         vTaskDelay(1);
//     }
// }

esp_err_t start_i2c_slave(void)
{
    ESP_RETURN_ERROR(i2c_slave_init(PIN_I2C_S_SDA, PIN_I2C_S_SCL, I2C_SLAVE_ADDRESS));
    // BaseType_t res = xTaskCreatePinnedToCore(i2c_reader_task, "i2c_reader_task", 1024 * 4, NULL, 5, NULL, 0);
    // if (res != pdPASS) {
    //     return ESP_FAIL;
    // }
    // res = xTaskCreatePinnedToCore(i2c_writer_task, "i2c_writer_task", 1024 * 4, NULL, 5, NULL, 0);
    // if (res != pdPASS) {
    //     return ESP_FAIL;
    // }
    LOG_INFO("I2C slave ready! Address: 0x%x", I2C_SLAVE_ADDRESS);
    return ESP_OK;
}
#endif // ENABLE_I2C

#ifdef __cplusplus
}
#endif
