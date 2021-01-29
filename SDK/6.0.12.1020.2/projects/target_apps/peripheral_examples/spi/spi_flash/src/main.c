/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief SPI flash example
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

#include <stdio.h>
#include <stdint.h>
#include "arch_system.h"
#include "user_periph_setup.h"
#include "gpio.h"
#include "uart.h"
#include "uart_utils.h"
#include "spi.h"
#include "spi_flash.h"

uint8_t rd_data[512];
uint8_t wr_data[512];
uint8_t dev_id;

/**
 ****************************************************************************************
 * @brief SPI Flash test function
 * @return void
 ****************************************************************************************
 */
void spi_flash_test(void);

/**
 ****************************************************************************************
 * @brief Main routine of the SPI flash example
 * @return void
 ****************************************************************************************
 */
int main (void)
{
    system_init();
    periph_init();
    
    while(1){
			spi_flash_test();
		}
}

void spi_flash_test(void){
		
  spi_set_bitmode(SPI_MODE_16BIT);
	spi_cs_low();

	spi_access(0x8F00);
	
	spi_cs_high();
//	uint8_t u8pData_out[] = {0x08};
//	uint8_t u8pData_in[1];
//	
//	spi_transfer(u8pData_out,u8pData_in,1,SPI_OP_BLOCKING);

}
