#ifndef _DICE_LED_DRIVER_H_
#define _DICE_LED_DRIVER_H_

#include "gpio.h"

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

#define LED_SWITCH 	true //false
#define SIDE_SWITCH false  // true

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

#endif // _DICE_LED_DRIVER_H_
