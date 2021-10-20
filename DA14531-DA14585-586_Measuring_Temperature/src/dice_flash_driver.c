#include "dice_flash_driver.h"
#include "spi_flash.h"
#include "uart.h"
#include "uart_utils.h"
#include "user_periph_setup.h"

void test(){
	uint8_t u8aPage[SPI_FLASH_PAGE_SIZE];
	float write_buffef[NN_FLOATS_IN_PAGE];
	uint32_t actual_size = 0;
	
	for(float j = 1.0f; j<63.0f; j += 1.0f){
		write_buffef[actual_size] = j;
		actual_size++;
	}
	
	dice_flash_store_nn_data(write_buffef,NN_FLOATS_IN_PAGE,6);
	spi_flash_read_data_dma(u8aPage,NN_DATA_START,SPI_FLASH_PAGE_SIZE,&actual_size);
	
	for(uint16_t i = 0; i<SPI_FLASH_PAGE_SIZE; i++){
		printf_byte(UART, u8aPage[i]);
		printf_string(UART, " ");
	}
	
}

void dice_flash_init(void){
	printf_string(UART, "init");
	dice_flash_clear_nn_pages(); //remove if not testing
	u16Page_pointer = dice_flash_seek_start_page();
	test();
}

void dice_flash_store_nn_data(const float* data, const uint8_t size, const uint8_t result){
	if(size > NN_FLOATS_IN_PAGE){			return;}
	if((result > 6) || (result < 1)){	return;}
	if(u16Page_pointer >= NN_MAX_PAGES){return;}
	
	uint8_t u8aPage[SPI_FLASH_PAGE_SIZE];
	
	dice_flash_create_page(data,size,result,u8aPage);
	spi_flash_page_program_dma(u8aPage, dice_flash_get_start_add(&u16Page_pointer), SPI_FLASH_PAGE_SIZE);
	u16Page_pointer++;
}

/*
 * Static functions
 ****************************************************************************************
 */

static uint16_t dice_flash_seek_start_page(void){
	
	for(uint16_t page = 0; page<(NN_MAX_PAGES-1); page++){
		if(dice_flash_page_empty(&page)){
			return page;
		}
	}

	return -1;
}

static bool dice_flash_valid_page_start_add(const uint32_t* address){
	return (*address <= NN_LAST_PAGE_ADD);
}

static bool dice_flash_page_empty(const uint16_t *page_pointer){
	uint32_t u32Start_address = ((*page_pointer * SPI_FLASH_PAGE_SIZE) + NN_DATA_START);
	return (spi_flash_is_page_empty(u32Start_address) == SPI_FLASH_ERR_OK);
}

void dice_flash_periph_init(void){
	
	RESERVE_GPIO(SPI_SS, SPI_EN_GPIO_PORT, SPI_EN_GPIO_PIN, PID_SPI_EN);
	RESERVE_GPIO(SPI_MISO, SPI_DI_GPIO_PORT, SPI_DI_GPIO_PIN, PID_SPI_DI);
	RESERVE_GPIO(SPI_MOSI, SPI_DO_GPIO_PORT, SPI_DO_GPIO_PIN, PID_SPI_DO);
	RESERVE_GPIO(SPI_CLK, SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, PID_SPI_CLK);
	
	GPIO_ConfigurePin( SPI_EN_GPIO_PORT, SPI_EN_GPIO_PIN, OUTPUT, PID_SPI_EN, true); 
	GPIO_ConfigurePin( SPI_DI_GPIO_PORT, SPI_DI_GPIO_PIN, INPUT, PID_SPI_DI, true); 
	GPIO_ConfigurePin( SPI_DO_GPIO_PORT, SPI_DO_GPIO_PIN, OUTPUT, PID_SPI_DO, true); 
	GPIO_ConfigurePin( SPI_CLK_GPIO_PORT, SPI_CLK_GPIO_PIN, OUTPUT, PID_SPI_CLK, true);

	spi_flash_configure_env(&spi_flash_cfg);
	spi_initialize(&spi_cfg);
}

static void dice_flash_clear_nn_pages(void){
	uint16_t u16Page_count = dice_flash_seek_start_page();
	
	if(u16Page_count > NN_MAX_PAGES){return;}
	
	for(uint16_t i = 0; i<u16Page_count; i++){spi_flash_page_erase(dice_flash_get_start_add(&i));}
}

static void dice_flash_create_page(const float* data, const uint8_t size, const uint8_t result, uint8_t *u8aPage){
	
	for(uint8_t i = 0; i<size; i++){
		uint32_t u32Buffer;
		memcpy(&u32Buffer,&data[i],sizeof(u32Buffer));
		
		for(uint8_t j = 0; j<4; j++){u8aPage[(i*4)+j] = (uint8_t)(u32Buffer >> (j*8));}
	}
	
	u8aPage[NN_SIZE_POSITION] 	= size;
	u8aPage[NN_RESULT_POSITION] = result;
}

static uint32_t dice_flash_get_start_add(const uint16_t *page_pointer){
	return ((*page_pointer*SPI_FLASH_PAGE_SIZE)+NN_DATA_START);
}
