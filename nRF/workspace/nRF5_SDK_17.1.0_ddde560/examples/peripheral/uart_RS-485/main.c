/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "nrf_uart.h"

#define PIN_TX  11
#define PIN_RX  12
#define PIN_DE  13
#define PIN_RE  14
#define HIGH    1
#define LOW     0

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */



void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

int main(void)
{
    uint32_t err_code;


    bsp_board_init(BSP_INIT_LEDS);
  
    const app_uart_comm_params_t comm_params =
      {
          PIN_RX,
          PIN_TX,
          RTS_PIN_NUMBER, // rts_pin_no
          CTS_PIN_NUMBER, // cts_pin_no
          0,      // flow_control
          false,          // use_parity
          NRF_UART_BAUDRATE_115200 // baud_rate
      };

    APP_UART_FIFO_INIT(&comm_params,            
                         UART_RX_BUF_SIZE,      
                         UART_TX_BUF_SIZE,     
                         uart_error_handle,    
                         APP_IRQ_PRIORITY_LOWEST, 
                         err_code);               

    APP_ERROR_CHECK(err_code);

    char tmp = 0;

    nrf_gpio_cfg_output(PIN_DE);
    nrf_gpio_cfg_output(PIN_RE);
    nrf_gpio_pin_write(PIN_DE, HIGH);
    nrf_gpio_pin_write(PIN_RE, LOW);
    

    while (true)
    {
#if 0
        // RS-485 to NRF DK
        if (app_uart_get(&tmp) == NRF_SUCCESS)
        {
            printf("[  ] RS-485 Send --> %c\n", tmp);
        }
#else
        // NRF DK to RS-485 
        app_uart_put('a');
        app_uart_put('\n');
        nrf_delay_ms(1000);
#endif
    }
}


/** @} */
