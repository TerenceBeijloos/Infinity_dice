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
#include <time.h> 

#include "temp.h"

static bool bNormal_mode																__SECTION_ZERO("retention_mem_area0");
static uint8_t u8aDice_chance[DICE_SIDE_COUNT]					__SECTION_ZERO("retention_mem_area0");
static char caDice_chance_ntf_string[NTF_STRING_SIZE] 	__SECTION_ZERO("retention_mem_area0");

void dice_chance_init(void){
	
	bNormal_mode = false;								
	memset(u8aDice_chance,0x00,sizeof(u8aDice_chance));				
  memset(caDice_chance_ntf_string,0x00,sizeof(caDice_chance_ntf_string));	
	network_init();
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
	
	predict(prediction,input);
	float predict5 = prediction[5];
	
	static const uint16_t size = 7;
	char last_prediction[size];
	snprintf(last_prediction, size, "%u", sensor_data);
	
	struct custs1_val_ntf_ind_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
																				prf_get_task_from_id(TASK_ID_CUSTS1),
																				TASK_APP,
																				custs1_val_ntf_ind_req,
																				NTF_STRING_SIZE);
	
	req->conidx			 	= 0;                                        //Connection ID to send the data to (this application can only have one connection(0))
	req->notification = true;                               			//Data is sent as a notification and not as indication
	req->handle 			= DICE_CHANGE_CHANCE_VAL;      					    //The handle of the characteristic we want to write to
	req->length 			= NTF_STRING_SIZE;                          //Data length in bytes
	memcpy(req->value, last_prediction, size);//Copy the string to the message
		
	ke_msg_send(req);                                       			//Send the message to the task
}

void dice_chance_recieve_handler(struct custs1_val_write_ind const *param){
	
	const uint8_t u8Step_size = 3;
	uint8_t u8Percentage_sum 	= 0;
	
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
}
