#include "dice_sensor_driver.h"
#include "user_periph_setup.h"
#include "temp.h"

void dice_sensor_enable_magnetometer(void)
{
	if(sensor_selected != magnetometer)
	{
		sensor_selected = magnetometer;
		i2c_init(&i2c_magnetometer_cfg);
	}
}

void dice_sensor_enable_accelero_gyroscope(void)
{
	if(sensor_selected != accelero_gyroscope)
	{
		sensor_selected = accelero_gyroscope;
		i2c_init(&i2c_accelero_gyroscope_cfg);
	}
}

void dice_sensor_test(void){
	uint8_t u8aData = 0x01;
	i2c_abort_t *abrt_code;
	
	GPIO_SetActive(GPIO_LED_PORT, GPIO_LED_PIN);
	//dice_sensor_write_bits(CTRL_REG4,u8aData,abrt_code);
	uint8_t data = dice_sensor_read_byte(CTRL_REG4,abrt_code);
	GPIO_SetInactive(GPIO_LED_PORT, GPIO_LED_PIN);
	
}

/****************************************************************************************/
/* Init functions	                                                                    	*/
/****************************************************************************************/

static void dice_sensor_periph_init(void){
	RESERVE_GPIO(SDA,SDA_PORT, SDA_PIN, PID_I2C_SDA);
	RESERVE_GPIO(SCL,SCL_PORT, SCL_PIN, PID_I2C_SCL);
	
	GPIO_ConfigurePin(SCL_PORT, SCL_PIN, INPUT_PULLUP, PID_I2C_SCL, false);
	GPIO_ConfigurePin(SDA_PORT, SDA_PIN, INPUT_PULLUP, PID_I2C_SDA, false);
	
	sensor_selected = accelero_gyroscope;
	i2c_init(&i2c_accelero_gyroscope_cfg);
}
	
static void dice_sensor_var_init(void){

	uint8_t u8Index = 0;
	
//	u8Index = dice_sensor_add_limit(u8Index,ACT_THS, INT2_CTRL, u8aWrite_permissions);
//	u8Index = dice_sensor_add_limit(u8Index,CTRL_REG1_G, CTRL_REG3_G, u8aWrite_permissions);
//	u8Index = dice_sensor_add_limit(u8Index,CTRL_REG4, CTRL_REG10, u8aWrite_permissions);
//	u8Index = dice_sensor_add_limit(u8Index,FIFO_CTRL, FIFO_CTRL, u8aWrite_permissions);
//	u8Index = dice_sensor_add_limit(u8Index,INT_GEN_CFG_G, INT_GEN_DUR_G, u8aWrite_permissions);
	
}

void dice_sensor_init(void){
	dice_sensor_var_init();
	dice_sensor_periph_init();
}

/****************************************************************************************/
/* Write functions	                                                                  	*/
/****************************************************************************************/

void dice_sensor_write_bits(const uint8_t u8Address, const uint8_t u8Data, i2c_abort_t *abrt_code){
	
	dice_sensor_write_permission(u8Address,false);
	
	uint8_t u8Mask[ADDRESS_SIZE + 1]; //+ 1 = write 1 byte
	u8Mask[0] = u8Address;
	u8Mask[1] = (dice_sensor_read_byte(u8Address, abrt_code) | u8Data); 
	
	while(i2c_controler_is_busy()){}
	i2c_master_transmit_buffer_sync(u8Mask,(ADDRESS_SIZE+1),abrt_code,I2C_F_WAIT_FOR_STOP);
	
}

void dice_sensor_write_byte(const uint8_t u8Address, const uint8_t u8Data, i2c_abort_t *abrt_code){

	dice_sensor_write_permission(u8Address,false);
	
	uint8_t u8aBuffer[ADDRESS_SIZE + 1]; //+ 1 = write 1 byte
	u8aBuffer[0] = u8Address;
	u8aBuffer[1] = u8Data;
	
	while(i2c_controler_is_busy()){}
	i2c_master_transmit_buffer_sync(u8aBuffer,(ADDRESS_SIZE+1),abrt_code,I2C_F_WAIT_FOR_STOP);
	
}

//u8pData[0] must be the address of the register
void dice_sensor_write_multiple(const uint8_t *u8pData, const uint8_t u8Length, i2c_abort_t *abrt_code){
	
	const uint8_t u8StartAddress = u8pData[0];
	
	for(uint8_t i = u8StartAddress; i<(u8StartAddress+u8Length); i++){
		dice_sensor_write_permission(i,false);
	}
	
	while(i2c_controler_is_busy()){}
	i2c_master_transmit_buffer_sync(u8pData,u8Length,abrt_code,I2C_F_WAIT_FOR_STOP);
		
}


/****************************************************************************************/
/* Read functions		                                                                   	*/
/****************************************************************************************/

uint8_t dice_sensor_read_byte(const uint8_t u8Address, i2c_abort_t *abrt_code){
	
	if( dice_sensor_out_of_bound(u8Address,false)){
		*abrt_code = I2C_ABORT_SW_ERROR;
		return 0xFF;
	}
	
	uint8_t u8aBuffer[ADDRESS_SIZE];
	u8aBuffer[0] = u8Address;
	
	while(i2c_controler_is_busy()){}
	i2c_master_transmit_buffer_sync(u8aBuffer,ADDRESS_SIZE,abrt_code,I2C_F_WAIT_FOR_STOP);
	if(*abrt_code != I2C_ABORT_NONE){return 0xFF;}
	
	while(i2c_controler_is_busy()){}
	i2c_master_receive_buffer_sync(u8aBuffer,1,abrt_code,I2C_F_WAIT_FOR_STOP); //1 = read 1 byte
	if(*abrt_code != I2C_ABORT_NONE){return 0xFF;}
	
	return u8aBuffer[0];
}

void dice_sensor_read_multiple(const uint8_t u8StartAddress, uint8_t *u8pData, const uint8_t u8Length, i2c_abort_t *abrt_code){
	
	if( dice_sensor_out_of_bound(u8StartAddress,false) || dice_sensor_out_of_bound((u8StartAddress+u8Length-1),false) ){
		*abrt_code = I2C_ABORT_SW_ERROR;
		return;
	}

	uint8_t u8aWrite_buffer[ADDRESS_SIZE];
	u8aWrite_buffer[0] = u8StartAddress;
	
	while(i2c_controler_is_busy()){}
	i2c_master_transmit_buffer_sync(u8aWrite_buffer,ADDRESS_SIZE,abrt_code,I2C_F_WAIT_FOR_STOP);
	if(*abrt_code != I2C_ABORT_NONE){return;}
	
	while(i2c_controler_is_busy()){}
	i2c_master_receive_buffer_sync(u8pData,u8Length,abrt_code,I2C_F_WAIT_FOR_STOP);
		
}

/****************************************************************************************/
/* Support functions                                                                  	*/
/****************************************************************************************/

bool dice_sensor_out_of_bound(const uint8_t u8Address, bool bRecoverable){
	
	if(u8Address < ACT_THS || u8Address > INT_GEN_DUR_G){
		if(bRecoverable){
			return true;
		}
		//error
	}
	return false;
}

//TODO Implement binary search
bool dice_sensor_write_permission(const uint8_t u8Address, bool bRecoverable){
	return true;
	
//	for (uint8_t i = 0; i<PERMISSIONS_WRITE; i++){
//		if(u8aWrite_permissions[i] == u8Address){
//			return true;
//		}
//	}
//	
//	if(!bRecoverable){
//		//error
//	}
//	
//	return false;
}

static uint8_t dice_sensor_add_limit(uint8_t u8Array_start, uint8_t u8Address_start, uint8_t u8Address_end, uint8_t *u8pArray){
	
	uint8_t u8Index;
	
	for( u8Index = u8Array_start; u8Index<=(u8Array_start+(u8Address_end-u8Address_start) ); u8Index++){
		u8pArray[u8Index] = ((u8Index-u8Array_start) + u8Address_start );
	}
	
	return u8Index+1;
	
}
