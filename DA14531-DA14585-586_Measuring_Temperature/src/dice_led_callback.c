#include "dice_led_callback.h"

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "systick.h"
#include "dice_led_driver.h"
#include "arch_system.h"
#include "chacha20.h"
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
// timer_hnd thUpdate_led_numbers __SECTION_ZERO("retention_mem_area0");
NUMBER prev_number 			__SECTION_ZERO("retention_mem_area0");
NUMBER current_number		__SECTION_ZERO("retention_mem_area0");
uint8_t u8aKey[16]			__SECTION_ZERO("retention_mem_area0");

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
 
static NUMBER led_callback_get_next_number(const NUMBER number){
	
	if(number == six){
		return one;
	}
	return (number+1);
}

static void led_callback_dice_mode(void){
	
	led_turn_off(prev_number,led_get_side(prev_number));
	led_turn_on(current_number,led_get_side(current_number));
	
	prev_number 		= current_number;
	current_number 	= led_callback_get_next_number(current_number);
	
	led_callback_start_dice_mode();
}

void led_callback_start_dice_mode(void){

//	systick_stop();
	systick_register_callback(led_callback_dice_mode);
	systick_start(LED_UPDATE_TIMER, SYSTICK_EXCEPTION);
}

void led_update_key(void){
	
	for(uint8_t i = 0; i<4; i++){
		u8aKey[i] = (uint8_t)((systick_value()>>i)&0xFF);
	}
	
}

uint8_t get_random(void){
	led_update_key();
	csprng_seed(u8aKey); 
	return (uint8_t)(csprng_get_next_uint32()%100);
}

void led_callback_init(void){
	prev_number = one;
	current_number = led_callback_get_next_number(prev_number);
	//start callback last because it uses prev_number and current_number
	led_callback_start_dice_mode();
	
	uint8_t u8aKeyCpy[] = {0xfc, 0x1e, 0x11, 0xf9, 0x19, 0x3f, 0x4e, 0x76, 0x7f, 0xfe, 0xb0, 0x66, 0x3c, 0xb0, 0xb5, 0x07};
	memcpy(u8aKey,u8aKeyCpy,sizeof(u8aKey));
	
}

