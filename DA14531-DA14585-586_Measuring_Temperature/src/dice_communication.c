#include "dice_communication.h"
#include "app_api.h"
#include "app.h"
#include "prf_utils.h"
#include "custs1.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "dice_neural_network.h"
#include "systick.h"
#include "dice_sensor_driver.h"
#include <time.h> 

#include "temp.h"

ke_msg_id_t timer_temperature_ntf      		__SECTION_ZERO("retention_mem_area0"); 		
static bool bNormal_mode																__SECTION_ZERO("retention_mem_area0");
static uint8_t u8aDice_chance[DICE_SIDE_COUNT]					__SECTION_ZERO("retention_mem_area0");
static char caDice_chance_ntf_string[NTF_STRING_SIZE] 	__SECTION_ZERO("retention_mem_area0");

void dice_chance_init(void){
	
	bNormal_mode = false;								
	memset(u8aDice_chance,0x00,sizeof(u8aDice_chance));				
  memset(caDice_chance_ntf_string,0x00,sizeof(caDice_chance_ntf_string));	
	//network_init();
}

void dice_chance_deinit(void);

void dice_chance_set_ntf_string(const char caDice_chance[NTF_STRING_SIZE]){
	memcpy(caDice_chance_ntf_string,caDice_chance,sizeof(caDice_chance_ntf_string));
}

char *dice_chance_get_ntf_string(void){
	return caDice_chance_ntf_string;
}

void dice_chance_prepare_ntf_string(void){
	
		uint8_t u8Step_size = 3;
	
		for(uint8_t i = 0; i<NTF_STRING_SIZE;i+=u8Step_size){
			caDice_chance_ntf_string[i+1] = '0' + (uint8_t)((u8aDice_chance[i/u8Step_size]%1000)/100);
			caDice_chance_ntf_string[i+2] = '0'	+	(uint8_t)((u8aDice_chance[i/u8Step_size]%100 )/10 );
			caDice_chance_ntf_string[i+3] = '0'	+	(uint8_t) (u8aDice_chance[i/u8Step_size]%10			 	);
		}
//	caDice_chance_ntf_string[sizeof(caDice_chance_ntf_string)-1] = '\0';
}

void dice_chance_send(void){
	
//	dice_chance_prepare_ntf_string();

	float prediction[6];
	float input[] = { 0.013893f, 0.998055f, 0.146985f, 0.560383f, 0.357070f, 0.638900f };
	
	//predict(prediction,input);
	float predict5 = prediction[5];
	
	static const uint16_t size = 30;
	char last_prediction[NTF_STRING_SIZE];
	
	read_magnetometer();
	uint8_t length = snprintf(last_prediction, NTF_STRING_SIZE, "(%.0f,%.0f,%.0f)", sensor_data.x,sensor_data.y,sensor_data.z);
	
	struct custs1_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																				prf_get_task_from_id(TASK_ID_CUSTS1),
																				TASK_APP,
																				custs1_val_ntf_ind_req,
																				NTF_STRING_SIZE);
	
	req->conidx			 	= 0;                                        //Connection ID to send the data to (this application can only have one connection(0))
	req->notification = true;                               			//Data is sent as a notification and not as indication
	req->handle 			= DICE_CHANGE_CHANCE_VAL;      					    //The handle of the characteristic we want to write to
	req->length 			= length;                          //Data length in bytes
	memcpy(req->value, last_prediction, length);//Copy the string to the message
		
	ke_msg_send(req);                                       			//Send the message to the task
	timer_temperature_ntf = app_easy_timer(NOTIFICATION_DELAY/10, dice_chance_send); //Set a timer for NOTIFICATION_DELAY ms
}

void dice_chance_recieve_handler(struct custs1_val_write_ind const *param){
	
	const uint8_t u8Step_size = 3;
	uint8_t u8Percentage_sum 	= 0;
	
	if(param->length >= 1 && param->value[0] == 69)
	{
		//wdg_resume();
		//spi_flash_chip_erase(); //Change this if data other then the boot image is stored
		//SCB->AIRCR = 0x05FA0004;
    // Trigger SW reset
    uint16_t tmp = GetWord16(SYS_CTRL_REG);
    tmp = (tmp & ~REMAP_ADR0) | 0; // Map ROM at address 0
    tmp |= SW_RESET;
    SetWord16(SYS_CTRL_REG, tmp);
		//for(;;);
	}
	if(param->length == 1){
		bNormal_mode = true;
	}else if(param->length < 18){
		//error_handler();
	}
	
	bNormal_mode = false;
	for(uint8_t i = 0; i<param->length; i+=u8Step_size){
		uint8_t u8Value 							 = ((param->value[i]-'0')*100) + ((param->value[i+1]-'0')*10) + ((param->value[i+2]-'0'))+1;
		u8Percentage_sum 							+= u8Value;
		u8aDice_chance[i/u8Step_size]  = u8Value;
	}
	
	
	if(u8Percentage_sum != 100){
		//error_handler();
	}
	
	dice_chance_send();
	
	if(timer_temperature_ntf == EASY_TIMER_INVALID_TIMER){ 
		//Start the timer if it is not running
		timer_temperature_ntf = app_easy_timer(NOTIFICATION_DELAY/10, dice_chance_send); //Set a timer for NOTIFICATION_DELAY ms
	}
}
