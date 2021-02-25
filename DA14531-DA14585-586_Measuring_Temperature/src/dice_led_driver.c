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
	
	/*LED_DIAGANOL_LTR
	LED_DIAGANOL_RTL
	LED_MIDDLE_PAIR 
	LED_MIDDLE_ONE*/
	
	RESERVE_GPIO(LED,LED_DIAGANOL_LTR.gpPort,LED_DIAGANOL_LTR.gpPin,PID_GPIO);
	GPIO_ConfigurePin(LED_DIAGANOL_LTR.gpPort,LED_DIAGANOL_LTR.gpPin,OUTPUT,PID_GPIO, !LED_SWITCH);
	
	RESERVE_GPIO(LED,LED_DIAGANOL_RTL.gpPort,LED_DIAGANOL_RTL.gpPin,PID_GPIO);
	GPIO_ConfigurePin(LED_DIAGANOL_RTL.gpPort,LED_DIAGANOL_RTL.gpPin,OUTPUT,PID_GPIO, !LED_SWITCH);
	
	RESERVE_GPIO(LED,LED_MIDDLE_PAIR.gpPort,LED_MIDDLE_PAIR.gpPin,PID_GPIO);
	GPIO_ConfigurePin(LED_MIDDLE_PAIR.gpPort,LED_MIDDLE_PAIR.gpPin,OUTPUT,PID_GPIO, !LED_SWITCH);
	
	RESERVE_GPIO(LED,LED_MIDDLE_ONE.gpPort,LED_MIDDLE_ONE.gpPin,PID_GPIO);
	GPIO_ConfigurePin(LED_MIDDLE_ONE.gpPort,LED_MIDDLE_ONE.gpPin,OUTPUT,PID_GPIO, !LED_SWITCH);
	
//	for (uint8_t i = 0; i<LED_PATERNS[six].u8Size; i++){
//		GPIO_PORT port = LED_PATERNS[six].LED_PATERN[i].gpPort;
//		GPIO_PIN pin 	 = LED_PATERNS[six].LED_PATERN[i].gpPin;
//		RESERVE_GPIO(led,port, pin,PID_GPIO);
//		GPIO_ConfigurePin(port, pin, OUTPUT, PID_GPIO, !LED_SWITCH);
//	}

}

void led_pair_turn_on(const GPIO_LED pair){
			if(LED_SWITCH){
			GPIO_SetActive(pair.gpPort,pair.gpPin);
		}else{
			GPIO_SetInactive(pair.gpPort,pair.gpPin);
		}
}

void led_pair_turn_off(const GPIO_LED pair){
			if(!LED_SWITCH){
			GPIO_SetActive(pair.gpPort,pair.gpPin);
		}else{
			GPIO_SetInactive(pair.gpPort,pair.gpPin);
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
