#ifndef _DICE_LED_DRIVER_H_
#define _DICE_LED_DRIVER_H_

#include "gpio.h"

typedef enum{
	one = 0,	
	two,	three,
	four,	five,	
	six
} NUMBER;

typedef enum{
	top = 0,
	bottom,
	left,
	right,
	front,
	back
} SIDES;

typedef struct{
		GPIO_PORT gpPort;
		GPIO_PIN	gpPin;
} GPIO_LED;

typedef GPIO_LED GPIO_SIDE;

typedef struct{
	uint8_t 	u8Size;
	GPIO_LED	LED_PATERN[4];
} PATERN;

static const GPIO_SIDE SIDE_TOP 					= {GPIO_PORT_0,GPIO_PIN_0};
static const GPIO_SIDE SIDE_BOT 					= {GPIO_PORT_0,GPIO_PIN_3};
static const GPIO_SIDE SIDE_LEFT 					= {GPIO_PORT_0,GPIO_PIN_7};
static const GPIO_SIDE SIDE_RIGHT 				= {GPIO_PORT_0,GPIO_PIN_6};
static const GPIO_SIDE SIDE_FRONT 				= {GPIO_PORT_0,GPIO_PIN_5};
static const GPIO_SIDE SIDE_BACK 					= {GPIO_PORT_0,GPIO_PIN_4};

static const GPIO_LED LED_DIAGANOL_LTR 		= {GPIO_PORT_1,GPIO_PIN_0};
static const GPIO_LED LED_DIAGANOL_RTL 		= {GPIO_PORT_1,GPIO_PIN_1};
static const GPIO_LED LED_MIDDLE_PAIR 		= {GPIO_PORT_1,GPIO_PIN_3};
static const GPIO_LED LED_MIDDLE_ONE 			= {GPIO_PORT_0,GPIO_PIN_2};

static 			 PATERN LED_PATERNS[6] 				__SECTION_ZERO("retention_mem_area0");
static			 GPIO_SIDE SIDE_CONTAINER[6]	__SECTION_ZERO("retention_mem_area0");



void led_periph_init(void);
void led_periph_deinit(void);

void led_toggle(const struct LED*);

#endif // _DICE_LED_DRIVER_H_
