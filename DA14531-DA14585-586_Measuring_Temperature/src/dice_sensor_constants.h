#ifndef _DICE_SENSOR_CONSTANTS_H_
#define _DICE_SENSOR_CONSTANTS_H_

/****************************************************************************************/
/* I2C configuration                                                                  	*/
/****************************************************************************************/

#define I2C_SLAVE_ADDRESS (0xD6>>1)                  // Set slave device address
#define I2C_SPEED_MODE    I2C_SPEED_FAST        // Speed mode: 			I2C_SPEED_FAST (400 kbits/s)
#define I2C_ADDRESS_MODE  I2C_ADDRESSING_7B     // Addressing mode: I2C_ADDRESSING_7B
#define I2C_ADDRESS_SIZE  I2C_1BYTES_ADDR       // Address width: 	I2C_1BYTE_ADDR 
	
#define SCL_PORT  				GPIO_PORT_0
#define SCL_PIN	  				GPIO_PIN_7
#define SDA_PORT  				GPIO_PORT_1
#define SDA_PIN   				GPIO_PIN_2


/****************************************************************************************/
/* Sensor registers                                                                   	*/
/****************************************************************************************/

#define ACT_THS 					0x04
#define ACT_DUR 					0x05

#define INT_GEN_CFG_XL 		0x06
#define INT_GEN_THS_X_XL 	0x07
#define INT_GEN_THS_Y_XL 	0x08
#define INT_GEN_THS_Z_XL 	0x09
#define INT_GEN_DUR_XL 		0x0A

#define REFERENCE_G 			0x0B	

#define INT1_CTRL 				0x0C
#define INT2_CTRL 				0x0D

#define WHO_AM_I 					0x0F

#define CTRL_REG1_G 			0x10
#define CTRL_REG2_G 			0x11
#define CTRL_REG3_G 			0x13
#define ORIENT_CFG_G 			0x13
#define INT_GEN_SRC_G 		0x14

#define OUT_TEMP_L  			0x15
#define OUT_TEMP_H 				0x16

#define STATUS_REG 				0x17

#define OUT_X_L_G 				0x18
#define OUT_X_H_G 				0x19
#define OUT_Y_L_G 				0x1A
#define OUT_Y_H_G 				0x1B
#define OUT_Z_L_G 				0x1C
#define OUT_Z_H_G 				0x1D

#define CTRL_REG4 				0x1E
#define CTRL_REG5_XL 			0x1F
#define CTRL_REG6_XL 			0x20
#define CTRL_REG7_XL 			0x21
#define CTRL_REG8 				0x22
#define CTRL_REG9 				0x23
#define CTRL_REG10 				0x24

#define INT_GEN_SRC_XL 		0x26

//#define STATUS_REG 			0x27

#define OUT_X_L_XL 				0x28
#define OUT_X_H_XL 				0x29
#define OUT_Y_L_XL 				0x2A
#define OUT_Y_H_XL 				0x2B
#define OUT_Z_L_XL 				0x2C
#define OUT_Z_H_XL 				0x2D

#define FIFO_CTRL 				0x2E
#define FIFO_SRC 					0x2F

#define INT_GEN_CFG_G 		0x30
#define INT_GEN_THS_XH_G 	0x31
#define INT_GEN_THS_XL_G 	0x32
#define INT_GEN_THS_YH_G 	0x33
#define INT_GEN_THS_YL_G 	0x34
#define INT_GEN_THS_ZH_G 	0x35
#define INT_GEN_THS_ZL_G 	0x36
#define INT_GEN_DUR_G 		0x37
 

#endif // _DICE_SENSOR_CONSTANTS_H_
