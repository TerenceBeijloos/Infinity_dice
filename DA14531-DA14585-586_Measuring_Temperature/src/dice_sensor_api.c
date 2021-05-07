#include "dice_sensor_api.h"
#include "dice_sensor_constants.h"
#include "i2c.h"
#include "dice_sensor_driver.h"
#include "systick.h"

void init_gyroscope(void)
{
	dice_sensor_enable_accelero_gyroscope();
	uint8_t tempRegValue = 0;
	i2c_abort_t abrt_code;
	
	// CTRL_REG1_G (Default value: 0x00)
	// [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
	// ODR_G[2:0] - Output data rate selection
	// FS_G[1:0] - Gyroscope full-scale selection
	// BW_G[1:0] - Gyroscope bandwidth selection
	
	// To disable gyro, set sample rate bits to 0. We'll only set sample
	// rate if the gyro is enabled.

	// gyro sample rate: value between 1-6
	// 1 = 14.9    4 = 238
	// 2 = 59.5    5 = 476
	// 3 = 119     6 = 952
	uint8_t sampleRate = 6;
	tempRegValue = (sampleRate & 0x07) << 5;

	dice_sensor_write_byte(CTRL_REG1_G, tempRegValue, &abrt_code);
	
	// CTRL_REG2_G (Default value: 0x00)
	// [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
	// INT_SEL[1:0] - INT selection configuration
	// OUT_SEL[1:0] - Out selection configuration
	dice_sensor_write_byte(CTRL_REG2_G, 0x00, &abrt_code);	
	
	
	// CTRL_REG4 (Default value: 0x38)
	// [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
	// Zen_G - Z-axis output enable (0:disable, 1:enable)
	// Yen_G - Y-axis output enable (0:disable, 1:enable)
	// Xen_G - X-axis output enable (0:disable, 1:enable)
	// LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
	// 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)
	tempRegValue = 0;
	tempRegValue |= (1<<5);
	tempRegValue |= (1<<4);
	tempRegValue |= (1<<3);
	tempRegValue |= (1<<1);
	dice_sensor_write_byte(CTRL_REG4, tempRegValue, &abrt_code);
}

void init_accelerometer(void)
{
	dice_sensor_enable_accelero_gyroscope();
	uint8_t tempRegValue = 0;
	i2c_abort_t abrt_code;
	
	//	CTRL_REG5_XL (0x1F) (Default value: 0x38)
	//	[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	//	DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
	//		00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
	//	Zen_XL - Z-axis output enabled
	//	Yen_XL - Y-axis output enabled
	//	Xen_XL - X-axis output enabled
	tempRegValue |= (1<<5);
	tempRegValue |= (1<<4);
	tempRegValue |= (1<<3);
	
	dice_sensor_write_byte(CTRL_REG5_XL, tempRegValue, &abrt_code);
	
	// CTRL_REG6_XL (0x20) (Default value: 0x00)
	// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	// ODR_XL[2:0] - Output data rate & power mode selection
	// FS_XL[1:0] - Full-scale selection
	// BW_SCAL_ODR - Bandwidth selection
	// BW_XL[1:0] - Anti-aliasing filter bandwidth selection
	tempRegValue = 0;
	// To disable the accel, set the sampleRate bits to 0.

	uint8_t sampleRate = 6;
	tempRegValue |= (sampleRate & 0x07) << 5;
		
	dice_sensor_write_byte(CTRL_REG6_XL, tempRegValue, &abrt_code);
	
	// CTRL_REG7_XL (0x21) (Default value: 0x00)
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	// HR - High resolution mode (0: disable, 1: enable)
	// DCF[1:0] - Digital filter cutoff frequency
	// FDS - Filtered data selection
	// HPIS1 - HPF enabled for interrupt function
}

void init_magnetometer(void)
{
	dice_sensor_enable_magnetometer();
	i2c_abort_t abrt_code;
	uint8_t tempRegValue = 0;
	
	// CTRL_REG1_M (Default value: 0x10)
	// [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][0][ST]
	// TEMP_COMP - Temperature compensation
	// OM[1:0] - X & Y axes op mode selection
	//	00:low-power, 01:medium performance
	//	10: high performance, 11:ultra-high performance
	// DO[2:0] - Output data rate selection
	// ST - Self-test enable
	uint8_t XYPerformance = 3;
	uint8_t sampleRate = 7;
	tempRegValue |= (XYPerformance & 0x3) << 5;
	tempRegValue |= (sampleRate & 0x7) << 2;
	dice_sensor_write_byte(CTRL_REG1_M, tempRegValue, &abrt_code);
	
	// CTRL_REG2_M (Default value 0x00)
	// [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
	// FS[1:0] - Full-scale configuration
	// REBOOT - Reboot memory content (0:normal, 1:reboot)
	// SOFT_RST - Reset config and user registers (0:default, 1:reset)
	tempRegValue = 0;
	dice_sensor_write_byte(CTRL_REG2_M, tempRegValue, &abrt_code); // +/-4Gauss
	
	// CTRL_REG3_M (Default value: 0x03)
	// [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
	// I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
	// LP - Low-power mode cofiguration (1:enable)
	// SIM - SPI mode selection (0:write-only, 1:read/write enable)
	// MD[1:0] - Operating mode
	//	00:continuous conversion, 01:single-conversion,
	//  10,11: Power-down
	tempRegValue = 0;
	uint8_t operatingMode = 0;
	tempRegValue |= (operatingMode & 0x3);
	dice_sensor_write_byte(CTRL_REG3_M, tempRegValue, &abrt_code); // Continuous conversion mode
	
	// CTRL_REG4_M (Default value: 0x00)
	// [0][0][0][0][OMZ1][OMZ0][BLE][0]
	// OMZ[1:0] - Z-axis operative mode selection
	//	00:low-power mode, 01:medium performance
	//	10:high performance, 10:ultra-high performance
	// BLE - Big/little endian data
	tempRegValue = 0;
	uint8_t ZPerformance = 3;
	tempRegValue = (ZPerformance & 0x3) << 2;
	dice_sensor_write_byte(CTRL_REG4_M, tempRegValue, &abrt_code);
	
	// CTRL_REG5_M (Default value: 0x00)
	// [0][BDU][0][0][0][0][0][0]
	// BDU - Block data update for magnetic data
	//	0:continuous, 1:not updated until MSB/LSB are read
	tempRegValue = 0;
	dice_sensor_write_byte(CTRL_REG5_M, tempRegValue, &abrt_code);
}


void dice_sensor_api_set_roll_interupt(void){
	i2c_abort_t i2c_abort_code = I2C_ABORT_NONE;
	dice_sensor_write_bits(INT_GEN_CFG_XL, SIX_D_BIT, &i2c_abort_code);
	
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
	
	while(!(dice_sensor_read_byte(STATUS_REG_1, &i2c_abort_code) & GDA_BIT)){}
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
	