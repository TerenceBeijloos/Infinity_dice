#include "dice_led_callback.h"

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "systick.h"
#include "dice_led_driver.h"
#include "arch_system.h"
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
// timer_hnd thUpdate_led_numbers __SECTION_ZERO("retention_mem_area0");
NUMBER prev_number 			__SECTION_ZERO("retention_mem_area0");
NUMBER current_number		__SECTION_ZERO("retention_mem_area0");

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

void led_callback_init(void){
	prev_number = one;
	current_number = led_callback_get_next_number(prev_number);
	//start callback last because it uses prev_number and current_number
	led_callback_start_dice_mode();
}

