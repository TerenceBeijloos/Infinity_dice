#ifndef _DICE_SENSOR_DRIVER_
#define _DICE_SENSOR_DRIVER_

#include "stdint.h"
#include "dice_sensor_constants.h"
#include "i2c.h"
#include "compiler.h"

static uint8_t u8aWrite_permissions[30] __SECTION_ZERO("retention_mem_area0");

static const i2c_cfg_t i2c_cfg = {
    .clock_cfg.ss_hcnt = I2C_SS_SCL_HCNT_REG_RESET,
    .clock_cfg.ss_lcnt = I2C_SS_SCL_LCNT_REG_RESET,
    .clock_cfg.fs_hcnt = I2C_FS_SCL_HCNT_REG_RESET,
    .clock_cfg.fs_lcnt = I2C_FS_SCL_LCNT_REG_RESET,
    .restart_en = I2C_RESTART_ENABLE,
    .speed = I2C_SPEED_MODE,
    .mode = I2C_MODE_MASTER,
    .addr_mode = I2C_ADDRESS_MODE,
    .address = I2C_SLAVE_ADDRESS,
    .tx_fifo_level = 1,
    .rx_fifo_level = 1,
};

void dice_sensor_add_limit(uint8_t u8Array_start, uint8_t u8Address_start, uint8_t u8Address_end, uint8_t *u8pArray);

void dice_sensor_var_init(void);
void dice_sensor_periph_init(void);

void dice_dice_sensor_write_bits(uint8_t u8Address, uint8_t u8Data, i2c_abort_t *abrt_code);
void dice_dice_sensor_write_byte(uint8_t u8Address, uint8_t u8Data, i2c_abort_t *abrt_code); 
void dice_sensor_write_multiple(uint8_t u8StartAddress, uint8_t *u8pData, uint8_t u8Length, i2c_abort_t *abrt_code);
 
uint8_t dice_sensor_read_byte(int u8Address, i2c_abort_t *abrt_code);
void 		dice_sensor_read_multiple(uint8_t u8StartAddress, uint8_t *u8pData, uint8_t u8Length);


#endif // _DICE_SENSOR_DRIVER_
