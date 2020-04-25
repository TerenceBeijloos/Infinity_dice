#ifndef _DICE_FLASH_DRIVER_H_
#define _DICE_FLASH_DRIVER_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include <stdint.h> 
#include <stdbool.h>
#include "compiler.h"
#include "spi_flash.h"


/*
 * DEFINES
 ****************************************************************************************
 */
#define NN_DATA_START 			0x7CFF //32kb (max size of bin) - 1 = 0x7CFF
#define NN_LAST_PAGE_ADD		0X3CF90
#define NN_SIZE_POSITION    (SPI_FLASH_PAGE_SIZE-4)		 //The amount of floats programmed in the page
#define NN_RESULT_POSITION  (SPI_FLASH_PAGE_SIZE-3)		 //The side facing up	
#define NN_FLOATS_IN_PAGE		63		 //252/4 (float = 4 bytes)
#define NN_MAX_PAGES				851		 //2mbit / 256
#define NN_FLASH_SIZE				250000

#define SPI_EN_GPIO_PORT    GPIO_PORT_0
#define SPI_EN_GPIO_PIN     GPIO_PIN_3

#define SPI_CLK_GPIO_PORT   GPIO_PORT_0
#define SPI_CLK_GPIO_PIN    GPIO_PIN_0

#define SPI_DO_GPIO_PORT    GPIO_PORT_0
#define SPI_DO_GPIO_PIN     GPIO_PIN_6

#define SPI_DI_GPIO_PORT    GPIO_PORT_0
#define SPI_DI_GPIO_PIN     GPIO_PIN_5

#define SPI_MS_MODE         SPI_MS_MODE_MASTER
#define SPI_CP_MODE         SPI_CP_MODE_0
#define SPI_WSZ             SPI_MODE_8BIT
#define SPI_CS              SPI_CS_0
#define SPI_SPEED_MODE      SPI_SPEED_MODE_4MHz
#if defined (CFG_SPI_DMA_SUPPORT)
	#define USER_SPI_DMA_CHANNEL  SPI_DMA_CHANNEL_01
#endif

static const spi_cfg_t spi_cfg = {
    .spi_ms = SPI_MS_MODE,
    .spi_cp = SPI_CP_MODE,
    .spi_speed = SPI_SPEED_MODE,
    .spi_wsz = SPI_WSZ,
    .spi_cs = SPI_CS,
    .cs_pad.port = SPI_EN_GPIO_PORT,
    .cs_pad.pin = SPI_EN_GPIO_PIN,
#if defined (__DA14531__)
    .spi_capture = SPI_EDGE_CAPTURE,
#endif
#if defined (CFG_SPI_DMA_SUPPORT)
    .spi_dma_channel = USER_SPI_DMA_CHANNEL,
    .spi_dma_priority = DMA_PRIO_0,
#endif
};

// Configuration struct for SPI FLASH
static const spi_flash_cfg_t spi_flash_cfg = {
    .chip_size = NN_FLASH_SIZE,
};

static uint16_t u16Page_pointer 		__SECTION_ZERO("retention_mem_area0");


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void dice_flash_init(void);
void dice_flash_store_nn_data(const float* data, const uint8_t size, const uint8_t result);

/*
 * STATIC FUNCTION DECLARATIONS
 ****************************************************************************************
 */
static uint16_t dice_flash_seek_start_page(void);
static bool dice_flash_page_empty(const uint16_t *page_pointer);

static uint32_t dice_flash_get_start_add(const uint16_t *page_pointer);
static bool dice_flash_valid_page_start_add(const uint32_t* address);

void dice_flash_periph_init(void);

static void dice_flash_clear_nn_pages(void);
static void dice_flash_create_page(const float* data, const uint8_t size, const uint8_t result, uint8_t *u8aPage);

#endif // _DICE_FLASH_DRIVER_H_
