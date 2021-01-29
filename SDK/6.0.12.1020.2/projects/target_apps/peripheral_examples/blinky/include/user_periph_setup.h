/**
 ****************************************************************************************
 *
 * @file user_periph_setup.h
 *
 * @brief Peripherals setup header file.
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

#ifndef _USER_PERIPH_SETUP_H_
#define _USER_PERIPH_SETUP_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "uart.h"
#include "gpio.h"

/****************************************************************************************/
/* UART2 configuration to print messages                                                */
/****************************************************************************************/
#define UART                        UART2
// Define UART2 Tx Pad
#if defined (__DA14531__)
    #define UART2_TX_PORT           GPIO_PORT_0
    #define UART2_TX_PIN            GPIO_PIN_6
#else
//    #define UART2_TX_PORT           GPIO_PORT_0
//    #define UART2_TX_PIN            GPIO_PIN_4
#endif

// Define UART2 Settings
#define UART2_BAUDRATE              UART_BAUDRATE_115200
#define UART2_DATABITS              UART_DATABITS_8
#define UART2_PARITY                UART_PARITY_NONE
#define UART2_STOPBITS              UART_STOPBITS_1
#define UART2_AFCE                  UART_AFCE_DIS
#define UART2_FIFO                  UART_FIFO_EN
#define UART2_TX_FIFO_LEVEL         UART_TX_FIFO_LEVEL_0
#define UART2_RX_FIFO_LEVEL         UART_RX_FIFO_LEVEL_0

/****************************************************************************************/
/* LED configuration                                                                    */
/****************************************************************************************/
// Define UART2 Tx Pad
//#if defined (__DA14531__)
//    #define LED_PORT                GPIO_PORT_0
//    #define LED_PIN                 GPIO_PIN_9
//#else
//    #define LED_PORT                GPIO_PORT_1
//    #define LED_PIN                 GPIO_PIN_0
//#endif



typedef enum{
	one = 0,	
	two,	three,
	four,	five,	
	six
} NUMBER;

//Order must be kept like this because numbers are linked to sides with the number of the side, see led_periph_init.
typedef enum{
	bottom = 0,
	left,
	front,
	back,
	right,
	top
} SIDES;

typedef struct{
		GPIO_PORT gpPort;
		GPIO_PIN	gpPin;
} GPIO_LED;

typedef GPIO_LED GPIO_SIDE;

typedef struct{
	uint8_t 	u8Size;
	GPIO_LED	LED_PATERN[3];
} PATERN;

#define LED_SWITCH 	false
#define SIDE_SWITCH true

/*
static const GPIO_SIDE SIDE_TOP 					= {GPIO_PORT_0,GPIO_PIN_0};
static const GPIO_SIDE SIDE_BOT 					= {GPIO_PORT_0,GPIO_PIN_3};
static const GPIO_SIDE SIDE_LEFT 					= {GPIO_PORT_0,GPIO_PIN_7};
static const GPIO_SIDE SIDE_RIGHT 				= {GPIO_PORT_0,GPIO_PIN_6};
static const GPIO_SIDE SIDE_FRONT 				= {GPIO_PORT_0,GPIO_PIN_5};
static const GPIO_SIDE SIDE_BACK 					= {GPIO_PORT_0,GPIO_PIN_4};
*/

static const GPIO_SIDE BOARD_TOP 						= {GPIO_PORT_2,GPIO_PIN_1}; // changed
static const GPIO_SIDE BOARD_MAIN 					= {GPIO_PORT_0,GPIO_PIN_6}; // changed
static const GPIO_SIDE BOARD_POWER 					= {GPIO_PORT_0,GPIO_PIN_2}; // changed
static const GPIO_SIDE BOARD_THREE 					= {GPIO_PORT_1,GPIO_PIN_0}; // changed
static const GPIO_SIDE BOARD_ONE 						= {GPIO_PORT_0,GPIO_PIN_7}; // changed
static const GPIO_SIDE BOARD_TWO 						= {GPIO_PORT_1,GPIO_PIN_1}; // changed

static const GPIO_LED LED_DIAGANOL_LTR 		= {GPIO_PORT_0,GPIO_PIN_5}; // changed
static const GPIO_LED LED_DIAGANOL_RTL 		= {GPIO_PORT_3,GPIO_PIN_0}; // changed
static const GPIO_LED LED_MIDDLE_PAIR 		= {GPIO_PORT_0,GPIO_PIN_4}; // changed
static const GPIO_LED LED_MIDDLE_ONE 			= {GPIO_PORT_0,GPIO_PIN_3}; // changed

static 			 PATERN 	 LED_PATERNS[6] 		__SECTION_ZERO("retention_mem_area0");
static			 GPIO_SIDE SIDE_CONTAINER[6]	__SECTION_ZERO("retention_mem_area0");
static			 SIDES 	 	 SIDE_OF_NUMBER[6] 	__SECTION_ZERO("retention_mem_area0");

void led_periph_init(void);
void led_periph_deinit(void);

void led_turn_on(const NUMBER number,const SIDES side);
void led_turn_off(const NUMBER number,const SIDES side);

PATERN 	led_get_patern(const NUMBER number);
SIDES 	led_get_side(const NUMBER number);
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Initializes application's peripherals and pins
 * @return  void
 ****************************************************************************************
 */
void periph_init(void);

#endif // _USER_PERIPH_SETUP_H_
