#include "dice_sensor_driver.h"

void dice_sensor_periph_init(void){
	RESERVE_GPIO(SDA,SDA_PORT, SDA_PIN, PID_I2C_SDA);
	RESERVE_GPIO(SCL,SCL_PORT, SCL_PIN, PID_I2C_SCL);
	
	GPIO_ConfigurePin(SCL_PORT, SCL_PIN, INPUT_PULLUP, PID_I2C_SCL, false);
  GPIO_ConfigurePin(SDA_PORT, SDA_PIN, INPUT_PULLUP, PID_I2C_SDA, false);
	
	i2c_init(&i2c_cfg);
}

void dice_sensor_add_limit(uint8_t u8Array_start, uint8_t u8Address_start, uint8_t u8Address_end, uint8_t *u8pArray){
	
	for(uint16_t i = u8Array_start; i<=(u8Array_start+(u8Address_end-u8Address_start) ); i++){
		u8pArray[i] = ((i-u8Array_start) + u8Address_start );
	}
	
}
	
void dice_sensor_var_init(void){

	
	
}
