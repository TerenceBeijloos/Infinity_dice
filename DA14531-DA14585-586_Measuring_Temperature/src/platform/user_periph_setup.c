/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
 *
 * Copyright (c) 2015-2019 Dialog Semiconductor. All rights reserved.
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_periph_setup.h"
#include "datasheet.h"
#include "system_library.h"
#include "rwip_config.h"
#include "gpio.h"
#include "uart.h"
#include "syscntl.h"
#include "dice_sensor_driver.h"
#include "dice_communication.h"
#include "dice_led_driver.h"
#include "dice_led_callback.h"
#include "dice_flash_driver.h"

/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

static const uart_cfg_t uart_cfg = {
    .baud_rate = UART2_BAUDRATE,
    .data_bits = UART2_DATABITS,
    .parity = UART2_PARITY,
    .stop_bits = UART2_STOPBITS,
    .auto_flow_control = UART2_AFCE,
    .use_fifo = UART2_FIFO,
    .tx_fifo_tr_lvl = UART2_TX_FIFO_LEVEL,
    .rx_fifo_tr_lvl = UART2_RX_FIFO_LEVEL,
    .intr_priority = 2,
};

#if DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{

    RESERVE_GPIO(UART2_TX, UART2_TX_PORT, UART2_TX_PIN, PID_UART2_TX);

//	RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);

}

#endif

void set_pad_functions(void)
{
#if defined (__DA14586__)
    // Disallow spontaneous DA14586 SPI Flash wake-up
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_3, OUTPUT, PID_GPIO, true);
#endif
    // Configure UART2 pin functionality
    GPIO_ConfigurePin(UART2_TX_PORT, UART2_TX_PIN, OUTPUT, PID_UART2_TX, false);
//	GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
}

void periph_init(void)
{
#if defined (__DA14531__)
    // In Boost mode enable the DCDC converter to supply VBAT_HIGH for the used GPIOs
    syscntl_dcdc_turn_on_in_boost(SYSCNTL_DCDC_LEVEL_3V0);

    // In ES1 DA14531 the debugger is left disabled by the booter when it loads the
    // application image to RAM.
#if defined (__ES1_DA14531__)
    syscntl_load_debugger_cfg();
#endif

#else
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    // ROM patch
    patch_func();
		
		    // Initialize UART2
    //uart_initialize(UART, &uart_cfg);
		//dice_flash_periph_init();
					
    // Set pad functionality
    //set_pad_functions();

		wdg_freeze();
//	  dice_sensor_init();
//		dice_chance_init();
			//led_periph_init();
					RESERVE_GPIO(idk1,GPIO_PORT_1,GPIO_PIN_0,PID_GPIO);
		GPIO_ConfigurePin(GPIO_PORT_1,GPIO_PIN_0, OUTPUT, PID_GPIO, !SIDE_SWITCH);
		
				RESERVE_GPIO(idk2,GPIO_PORT_0,GPIO_PIN_5,PID_GPIO);
		GPIO_ConfigurePin(GPIO_PORT_0,GPIO_PIN_5, OUTPUT, PID_GPIO, !SIDE_SWITCH);
    // Enable the pads
    GPIO_set_pad_latch_en(true);
		
//		dice_flash_init();
		//led_callback_init();
		/*led_turn_on(one,right);
		led_turn_on(two,right);
		led_turn_on(three,right);*/
		
		GPIO_SetActive(GPIO_PORT_1,GPIO_PIN_0); 
		GPIO_SetActive(GPIO_PORT_0,GPIO_PIN_5);
		//led_turn_on(four,left);
		//led_turn_on(two,left);
		//led_turn_on(three,left);
		
		for(;;);
		
}
