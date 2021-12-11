#include "dice_sensor_driver.h"
#include "user_periph_setup.h"
#include "temp.h"
#include "cmath"

static int16_t gBiasRaw[3]			__SECTION_ZERO("retention_mem_area0");
static int16_t aBiasRaw[3]			__SECTION_ZERO("retention_mem_area0");
static int16_t mBiasRaw[3]			__SECTION_ZERO("retention_mem_area0");

static float gBias[3]				__SECTION_ZERO("retention_mem_area0");
static float aBias[3]				__SECTION_ZERO("retention_mem_area0");
static float mBias[3]				__SECTION_ZERO("retention_mem_area0");

static int16_t gx					__SECTION_ZERO("retention_mem_area0");
static int16_t gy					__SECTION_ZERO("retention_mem_area0");
static int16_t gz					__SECTION_ZERO("retention_mem_area0");
static int16_t ax					__SECTION_ZERO("retention_mem_area0");
static int16_t ay					__SECTION_ZERO("retention_mem_area0");
static int16_t az					__SECTION_ZERO("retention_mem_area0");
static int16_t mx					__SECTION_ZERO("retention_mem_area0");
static int16_t my					__SECTION_ZERO("retention_mem_area0");
static int16_t mz					__SECTION_ZERO("retention_mem_area0");

#define M_PI 3.14159265359

typedef enum 
{
	FIFO_OFF = 0,
	FIFO_THS = 1,
	FIFO_CONT_TRIGGER = 3,
	FIFO_OFF_TRIGGER = 4,
	FIFO_CONT = 6
} fifoMode_type;

static void enableFIFO(bool enable)
{
	i2c_abort_t abrt_code;
	uint8_t temp = dice_sensor_read_byte(CTRL_REG9,&abrt_code);
	if (enable) temp |= (1<<1);
	else temp &= ~(1<<1);
	dice_sensor_write_byte(CTRL_REG9, temp, &abrt_code);
}

static void setFIFO(fifoMode_type fifoMode, uint8_t fifoThs)
{
	// Limit threshold - 0x1F (31) is the maximum. If more than that was asked
	// limit it to the maximum.
	i2c_abort_t abrt_code;
	uint8_t threshold = fifoThs <= 0x1F ? fifoThs : 0x1F;
	dice_sensor_write_byte(FIFO_CTRL, ((fifoMode & 0x7) << 5) | (threshold & 0x1F), &abrt_code);
}

static void readGyro()
{
	uint8_t buffer[6]; // We'll read six bytes from the gyro into temp
	i2c_abort_t abrt_code;
	dice_sensor_read_multiple(OUT_X_L_G, temp, sizeof(buffer), &abrt_code);
	if (abrt_code == I2C_ABORT_NONE) // Read 6 bytes, beginning at OUT_X_L_G
	{
		gx = (buffer[1] << 8) | buffer[0]; // Store x-axis values into gx
		gy = (buffer[3] << 8) | buffer[2]; // Store y-axis values into gy
		gz = (buffer[5] << 8) | buffer[4]; // Store z-axis values into gz

		gx -= gBiasRaw[X_AXIS];
		gy -= gBiasRaw[Y_AXIS];
		gz -= gBiasRaw[Z_AXIS];
	}
}

static void readAccel()
{
	uint8_t buffer[6]; // We'll read six bytes from the gyro into temp
	i2c_abort_t abrt_code;
	dice_sensor_read_multiple(OUT_X_L_XL, temp, sizeof(buffer), &abrt_code);

	if (abrt_code == I2C_ABORT_NONE) // Read 6 bytes, beginning at OUT_X_L_XL
	{
		ax = (temp[1] << 8) | temp[0]; // Store x-axis values into ax
		ay = (temp[3] << 8) | temp[2]; // Store y-axis values into ay
		az = (temp[5] << 8) | temp[4]; // Store z-axis values into az

		ax -= aBiasRaw[X_AXIS];
		ay -= aBiasRaw[Y_AXIS];
		az -= aBiasRaw[Z_AXIS];
	}
}
//TODO: add parameter with refrence to struct of something
void read_magnetometer(void)
{
	uint8_t temp[6]; // We'll read six bytes from the mag into temp	
	i2c_abort_t abrt_code;
	dice_sensor_read_multiple(OUT_X_L_M, temp, 6, &abrt_code);

	uint16_t mx = (temp[1] << 8) | temp[0];
	uint16_t my = (temp[3] << 8) | temp[2];
	uint16_t mz = (temp[5] << 8) | temp[4];

	float heading;
  if (my == 0)
    heading = (mx < 0) ? M_PI : 0;
  else
    heading = atan2(mx, my);

  heading -= -8.58 * M_PI / 180;

  if (heading > M_PI) heading -= (2 * M_PI);
  else if (heading < -M_PI) heading += (2 * M_PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / M_PI;
	//Divide by 100 to take up less digits
	sensor_data.x = mx/100;
	sensor_data.y = my/100;
	sensor_data.z = mz/100;
}

//TODO: magnetometer
void calibrate(void)
{
	uint8_t samples = 0;
	int ii;
	int32_t aBiasRawTemp[3] = {0, 0, 0};
	int32_t gBiasRawTemp[3] = {0, 0, 0};
	i2c_abort_t abrt_code;
	
	// Turn on FIFO and set threshold to 32 samples
	enableFIFO(true);
	setFIFO(FIFO_THS, 0x1F);
	
	while (samples < 0x1F)
	{
		samples = (dice_sensor_read_byte(FIFO_SRC, &abrt_code) & 0x3F); // Read number of stored samples
	}
	for(ii = 0; ii < samples ; ii++) 
	{	// Read the gyro data stored in the FIFO
		readGyro();
		gBiasRawTemp[0] += gx;
		gBiasRawTemp[1] += gy;
		gBiasRawTemp[2] += gz;
		readAccel();
		aBiasRawTemp[0] += ax;
		aBiasRawTemp[1] += ay;
		aBiasRawTemp[2] += az - (int16_t)(1./aRes); // Assumes sensor facing up!
	}  
	for (ii = 0; ii < 3; ii++)
	{
		gBiasRaw[ii] = gBiasRawTemp[ii] / samples;
		gBias[ii] = calcGyro(gBiasRaw[ii]);
		aBiasRaw[ii] = aBiasRawTemp[ii] / samples;
		aBias[ii] = calcAccel(aBiasRaw[ii]);
	}
	
	enableFIFO(false);
	setFIFO(FIFO_OFF, 0x00);
	
	if (autoCalc) _autoCalc = true;
}

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
/* Read functions		                                                                */
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
	*abrt_code = I2C_ABORT_NONE;
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
