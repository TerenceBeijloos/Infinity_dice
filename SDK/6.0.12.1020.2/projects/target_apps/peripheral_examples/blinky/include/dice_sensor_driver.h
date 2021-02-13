#ifndef _DICE_SENSOR_DRIVER_
#define _DICE_SENSOR_DRIVER_

#include "stdint.h"
#include "dice_sensor_constants.h"
#include "i2c.h"
#include "compiler.h"


static uint8_t u8aWrite_permissions[PERMISSIONS_WRITE] __SECTION_ZERO("retention_mem_area0");

static const i2c_cfg_t i2c_magneto_cfg = {
    .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
    .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
    .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
    .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
    .restart_en = I2C_RESTART_ENABLE,
    .speed = I2C_SPEED_MODE,
    .mode = I2C_MODE_MASTER,
    .addr_mode = I2C_ADDRESS_MODE,
    .address = I2C_MAGNETO_ADDRESS,
    .tx_fifo_level = 1,
    .rx_fifo_level = 1,
};

static const i2c_cfg_t i2c_gyro_accelero_cfg = {
    .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
    .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
    .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
    .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
    .restart_en = I2C_RESTART_ENABLE,
    .speed = I2C_SPEED_MODE,
    .mode = I2C_MODE_MASTER,
    .addr_mode = I2C_ADDRESS_MODE,
    .address = I2C_GRYO_ACCELERO_ADDRESS,
    .tx_fifo_level = 1,
    .rx_fifo_level = 1,
};

typedef enum{
	magneto,
	accelero,
	gyroscope
}Sensor;

void dice_sensor_test(void);

static uint8_t dice_sensor_add_limit(const uint8_t u8Array_start, const uint8_t u8Address_start, const uint8_t u8Address_end, uint8_t *u8pArray);

/****************************************************************************************/
/* Init functions                                                                  	*/
/****************************************************************************************/
static void dice_sensor_var_init		(void);
static void dice_sensor_periph_init	(void);
			 void dice_sensor_init				(void);

/****************************************************************************************/
/* Write functions                                                                     	*/
/****************************************************************************************/
//u8pData[0] must be the address of the register
void dice_sensor_write_multiple	(Sensor sensor, const uint8_t *u8pData, const uint8_t u8Length,i2c_abort_t *abrt_code);
void dice_sensor_write_bits			(Sensor sensor, const uint8_t u8Address,const uint8_t u8Data, 	i2c_abort_t *abrt_code);
void dice_sensor_write_byte			(const uint8_t u8Address,const uint8_t u8Data,  i2c_abort_t *abrt_code); 


/****************************************************************************************/
/* Read functions                                                                     	*/
/****************************************************************************************/
uint8_t dice_sensor_read_byte			(const uint8_t u8Address, i2c_abort_t *abrt_code);
void 		dice_sensor_read_multiple	(const uint8_t u8StartAddress, uint8_t *u8pData, const uint8_t u8Length, i2c_abort_t *abrt_code);

/****************************************************************************************/
/* Support functions                                                                  	*/
/****************************************************************************************/
static bool dice_sensor_write_permission(const uint8_t u8Address, bool bRecoverable);
static bool dice_sensor_out_of_bound		(const uint8_t u8Address, bool bRecoverable);
//static uint16_t dice_sensor_transmit(const uint8_t *data, uint16_t len, i2c_abort_t *abrt_code, uint32_t flags);
//static uint16_t dice_sensor_recieve(uint8_t *data, uint16_t len, i2c_abort_t *abrt_code, uint32_t flags);

#endif // _DICE_SENSOR_DRIVER_
