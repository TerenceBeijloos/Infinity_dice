/**
 ****************************************************************************************
 *
 * @file periph_setup.c
 *
 * @brief Peripherals initialization functions
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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_periph_setup.h"
#include "datasheet.h"
#include "gpio.h"
#include "uart.h"

#include "dice_led_driver.h"
//#include "dice_communication.h"
#include "gpio.h"


void led_periph_init(void){
	
	SIDE_CONTAINER[top] 		= BOARD_TOP;
	SIDE_CONTAINER[bottom] 	= BOARD_MAIN;
	SIDE_CONTAINER[left] 		= BOARD_POWER;
	SIDE_CONTAINER[right] 	= BOARD_THREE;
	SIDE_CONTAINER[front] 	= BOARD_ONE;
	SIDE_CONTAINER[back] 		= BOARD_TWO;
	
	LED_PATERNS[one].LED_PATERN[0] 		= LED_MIDDLE_ONE;
	LED_PATERNS[one].u8Size 					= 1;
	
	LED_PATERNS[two].LED_PATERN[0] 		= LED_DIAGANOL_RTL;
	LED_PATERNS[two].u8Size 					= 1;
	
	LED_PATERNS[three].LED_PATERN[0] 	= LED_DIAGANOL_RTL;
	LED_PATERNS[three].LED_PATERN[1] 	= LED_MIDDLE_ONE;
	LED_PATERNS[three].u8Size 				= 2;
	
	LED_PATERNS[four].LED_PATERN[0]  	= LED_DIAGANOL_RTL;
	LED_PATERNS[four].LED_PATERN[1]  	= LED_DIAGANOL_LTR;
	LED_PATERNS[four].u8Size 					= 2;
	
	LED_PATERNS[five].LED_PATERN[0]  	= LED_DIAGANOL_RTL;
	LED_PATERNS[five].LED_PATERN[1]  	= LED_DIAGANOL_LTR;
	LED_PATERNS[five].LED_PATERN[2]  	= LED_MIDDLE_ONE;
	LED_PATERNS[five].u8Size 					= 3;
	
	LED_PATERNS[six].LED_PATERN[0] 		= LED_DIAGANOL_RTL;
	LED_PATERNS[six].LED_PATERN[1] 		= LED_DIAGANOL_LTR;
	LED_PATERNS[six].LED_PATERN[2] 		= LED_MIDDLE_PAIR;
	LED_PATERNS[six].u8Size 					= 3;
	
	for (uint8_t i = 0; i<6; i++){
		GPIO_PORT port 	= SIDE_CONTAINER[i].gpPort;
		GPIO_PIN 	pin 	= SIDE_CONTAINER[i].gpPin;
		RESERVE_GPIO(side,port,pin,PID_GPIO);
		GPIO_ConfigurePin(port,pin, OUTPUT, PID_GPIO, !SIDE_SWITCH);
		SIDE_OF_NUMBER[i] = (SIDES)i;
	}
	
	RESERVE_GPIO(LED,LED_MIDDLE_ONE.gpPort,LED_MIDDLE_ONE.gpPin,PID_GPIO);
	GPIO_ConfigurePin(LED_MIDDLE_ONE.gpPort,LED_MIDDLE_ONE.gpPin,OUTPUT,PID_GPIO, !LED_SWITCH);
	
	for (uint8_t i = 0; i<LED_PATERNS[six].u8Size; i++){
		GPIO_PORT port = LED_PATERNS[six].LED_PATERN[i].gpPort;
		GPIO_PIN pin 	 = LED_PATERNS[six].LED_PATERN[i].gpPin;
		RESERVE_GPIO(led,port, pin,PID_GPIO);
		GPIO_ConfigurePin(port, pin, OUTPUT, PID_GPIO, !LED_SWITCH);
	}

}


void led_turn_on(const NUMBER number,const SIDES side){
	
	if(SIDE_SWITCH){
		GPIO_SetActive(SIDE_CONTAINER[side].gpPort,SIDE_CONTAINER[side].gpPin);
	}else{
		GPIO_SetInactive(SIDE_CONTAINER[side].gpPort,SIDE_CONTAINER[side].gpPin);
	}
	
	for(uint8_t i = 0; i<LED_PATERNS[number].u8Size; i++){
		if(LED_SWITCH){
			GPIO_SetActive(LED_PATERNS[number].LED_PATERN[i].gpPort,LED_PATERNS[number].LED_PATERN[i].gpPin);
		}else{
			GPIO_SetInactive(LED_PATERNS[number].LED_PATERN[i].gpPort,LED_PATERNS[number].LED_PATERN[i].gpPin);
		}
	}
	
}

void led_turn_off(const NUMBER number,const SIDES side){
	
	if(SIDE_SWITCH){
		GPIO_SetInactive(SIDE_CONTAINER[side].gpPort,SIDE_CONTAINER[side].gpPin);
	}else{
		GPIO_SetActive(SIDE_CONTAINER[side].gpPort,SIDE_CONTAINER[side].gpPin);
	}
	
	for(uint8_t i = 0; i<LED_PATERNS[number].u8Size; i++){
		if(LED_SWITCH){
			GPIO_SetInactive(LED_PATERNS[number].LED_PATERN[i].gpPort,LED_PATERNS[number].LED_PATERN[i].gpPin);
		}else{
			GPIO_SetActive(LED_PATERNS[number].LED_PATERN[i].gpPort,LED_PATERNS[number].LED_PATERN[i].gpPin);
		}
	}
	
}

PATERN led_get_patern(const NUMBER number){
	return LED_PATERNS[number];
}

SIDES led_get_side(const NUMBER number){
	return SIDE_OF_NUMBER[number];
}

static void set_pad_functions(void)
{
/*
    i.e. to set P0_1 as Generic purpose Output:
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_1, OUTPUT, PID_GPIO, false);
*/
	

// Configuration struct for UART2
//static const uart_cfg_t uart_cfg = {
//    .baud_rate = UART2_BAUDRATE,
//    .data_bits = UART2_DATABITS,
//    .parity = UART2_PARITY,
//    .stop_bits = UART2_STOPBITS,
//    .auto_flow_control = UART2_AFCE,
//    .use_fifo = UART2_FIFO,
//    .tx_fifo_tr_lvl = UART2_TX_FIFO_LEVEL,
//    .rx_fifo_tr_lvl = UART2_RX_FIFO_LEVEL,
//    .intr_priority = 2,
//};
}

void periph_init(void)
{
#if !defined (__DA14531__)
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));
    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);
#endif

    // Initialize UART2
//    uart_initialize(UART2, &uart_cfg);

		RESERVE_GPIO(idk1,BOARD_TWO.gpPort,BOARD_TWO.gpPin,PID_GPIO);
		GPIO_ConfigurePin(BOARD_TWO.gpPort,BOARD_TWO.gpPin, OUTPUT, PID_GPIO, true);
	
		RESERVE_GPIO(led,LED_MIDDLE_ONE.gpPort, LED_MIDDLE_ONE.gpPin,PID_GPIO);
		GPIO_ConfigurePin(LED_MIDDLE_ONE.gpPort, LED_MIDDLE_ONE.gpPin, OUTPUT, PID_GPIO, false);
  
	
    // Set pad functionality
    set_pad_functions();

    // Enable the pads
    GPIO_set_pad_latch_en(true);
}
