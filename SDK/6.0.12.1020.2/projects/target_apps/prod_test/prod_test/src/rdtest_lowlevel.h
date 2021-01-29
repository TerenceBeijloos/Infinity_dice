/**
 ****************************************************************************************
 *
 * @file rdtest_lowlevel.h
 *
 * @brief RD test low level functions header file.
 *
 * Copyright (c) 2012-2019 Dialog Semiconductor. All rights reserved.
 *
 * This software ("Software") is owned by Dialog Semiconductor.
 *
 * By using this Software you agree that Dialog Semiconductor retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Dialog Semiconductor is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Dialog Semiconductor products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * DIALOG SEMICONDUCTOR BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */

#ifndef _RDTEST_LOWLEVEL_H_
#define _RDTEST_LOWLEVEL_H_
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define DATAMASK     0xFF
#define DATASHIFT    0
#define ADDRESSMASK  0x0F
#define ADDRESSSHIFT 0
#define LATCHMASK    0x10
#define LATCHSHIFT   4

#define ENABLEMASK  0x20
#define ENABLESHIFT 5

#define VBAT_CTRL_ADDRESS     0 
#define UART_CONNECT_ADDRESS  2 
#define LOOPBACK_CTRL_ADDRESS 4
#define GATE_TO_UART_ADDRESS  6
#define VPP_CTRL_ADDRESS      8 
#define STARTGATE_ADDRESS     9
#define RESET_CTRL_REG        0x0A
#define PW_CTRL_ADDRESS       0x0B

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize ports
 *
 * This function initialize ports used for communication with CPLD the version can later 
 * be used if different communication methods are implemented, not yet used
 *
 * @param[in] version
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_init_ports(uint8_t version);

/**
 ****************************************************************************************
 * @brief Access to databus
 *
 * This function access to databus (8bit) = P3.7, P3.6, P3.5, P3.4, P3.3, P3.2, P3.1, P3.0
 *
 * @param[in] data
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_ll_write_data(uint8_t data);

/**
 ****************************************************************************************
 * @brief Access to addressbus
 *
 * This function access to addressbus (4bit) = P2.3, P2.2, P2.1, P2.0
 *
 * @param[in] address
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_ll_write_address(uint8_t address);

/**
 ****************************************************************************************
 * @brief Latch-pin
 *
 * This function access to latch-pin CPLD = P2.4
 *
 * @param[in] latch
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_ll_write_latch(uint8_t latch);

/**
 ****************************************************************************************
 * @brief Enable-pin
 *
 * This function access to enable-pin CPLD = P2.5
 *
 * @param[in] enable
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_ll_write_enable(uint8_t enable);

/**
 ****************************************************************************************
 * @brief Reset register
 *
 * This function access to reset register/bit in CPLD
 *
 * @param[in] reset
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_ll_reset(uint8_t reset);

/**
 ****************************************************************************************
 * @brief Update register
 *
 * This function update register, method: provide data+address, latch the result
 *
 * @param[in] address
 *
 * @param[in] data
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_updateregister(uint8_t address, uint8_t data);

/**
 ****************************************************************************************
 * @brief Enable the CPLD
 *
 * This function provide a enable to the CPLD, used for multi-address transfers
 * required after registers are updated... may be delayed...
 *
 * @return void
 *****************************************************************************************
 */
void rdtest_enable(void);

#endif // _RDTEST_LOWLEVEL_H_
