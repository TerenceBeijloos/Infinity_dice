#include "dice_sensor_api.h"
#include "dice_sensor_constants.h"
#include "i2c.h"
#include "dice_sensor_driver.h"
#include "systick.h"

void dice_sensor_api_set_roll_interupt(void){
	i2c_abort_t i2c_abort_code = I2C_ABORT_NONE;
//	dice_sensor_write_bits(INT_GEN_CFG_XL, SIX_D_BIT, &i2c_abort_code);
	
	dice_sensor_api_set_isr();
}

void dice_sensor_api_get_orientation(int16_t* vector){
	const uint8_t READ_LENGTH = (OUT_Z_H_G-OUT_X_L_G) + 1; //OUT_Z_H_G last address, OUT_X_L_G first address + 1 for the size
	uint8_t u8aData[READ_LENGTH]; 
	i2c_abort_t i2c_abort_code = I2C_ABORT_NONE;
	
	dice_sensor_read_multiple(OUT_X_L_G, u8aData, READ_LENGTH, &i2c_abort_code);
	
	if(i2c_abort_code != I2C_ABORT_NONE){
		//error
	}
	
	for(uint8_t i = 0; i<READ_LENGTH; i++){
		vector[i] = toInt(u8aData[i]);
	}
	
}

void dice_sensor_api_get_rotation_velocity(float* vector){
	int16_t vector1[3];
	int16_t vector2[3];
	uint32_t time = 0;
	
	dice_sensor_api_restart_systick();
	
	dice_sensor_api_get_orientation(vector1);
	dice_sensor_api_wait_for_gyroscope();
	dice_sensor_api_get_orientation(vector2);
	
	time = systick_value();
	
	dice_sensor_api_calculate_velocity(vector1, vector2, &time, vector);
}

void dice_sensor_api_roll_isr(void){
	
}

/****************************************************************************************/
/* Static functions                                                                     */
/****************************************************************************************/
static void dice_sensor_api_set_isr(void){
	GPIO_EnableIRQ(SIX_D_INT_PORT, SIX_D_INT_PIN, SIX_D_INT_TYPE, SIX_D_INT_INPUT_LVL, SIX_D_INT_EDGE_LVL, SIX_D_INT_DEBOUNCE);
	GPIO_SetIRQInputLevel(SIX_D_INT_TYPE, SIX_D_INT_START_IRQ);
	GPIO_RegisterCallback(SIX_D_INT_TYPE, dice_sensor_api_roll_isr);
}

static int16_t toInt(uint16_t twos_complement){
	    if ((twos_complement & 0x8000) == 0)
    {
        return twos_complement;
    }

    return (twos_complement & 0xEFFF) - 0xF000;
}

void dice_sensor_api_restart_systick(void){
	systick_start(-1, 0);
}

static void dice_sensor_api_wait_for_gyroscope(void){
	i2c_abort_t i2c_abort_code = I2C_ABORT_NONE;
	
	while(!(dice_sensor_read_byte(STATUS_REG, &i2c_abort_code) & GDA_BIT)){}
}

static void dice_sensor_api_calculate_velocity(const int16_t *vector1, const int16_t *vector2, const uint32_t *dTime, float *result){
	for(uint8_t i = 0; i<3; i++){result[i] = ((vector2[i] - vector1[i]) / *dTime);}
}
	
static void dice_sensor_api_store_roll(){
	float result[3];
	int16_t vector1[3];
	int16_t vector2[3];
	uint32_t time = 0;
	
	dice_sensor_api_get_orientation(vector1);
	
	//while(rolling){
	dice_sensor_api_restart_systick();
	
	dice_sensor_api_wait_for_gyroscope();
	dice_sensor_api_get_orientation(vector2);
	
	time = systick_value();
	dice_sensor_api_calculate_velocity(vector1, vector2, &time, result);
	
	//store_information
	memcpy(vector1,vector2,(sizeof(vector2)/sizeof(vector2[0])));
	//}
	
}
	