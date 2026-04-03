/*
 * fm_reader.c
 *
 *  Created on: Mar 22, 2026
 *      Author: Administrator
 */

#include "fm_reader.h"

#include "rb_spi.h"
#include <stdlib.h>


void fm_read_custom_data(uint32_t addr, uint8_t * buffer, uint32_t len){

	uint32_t flen = len + 5; // 5 bytes for command and address
	uint32_t * bufPtr = calloc(flen, sizeof(uint8_t));
	if(bufPtr == NULL) return;
	bufPtr[0] = 0x03; // Read command
	bufPtr[1] = (addr >> 16) & 0xFF; // Address byte 1
	bufPtr[2] = (addr >> 8) & 0xFF; // Address byte 2
	bufPtr[3] = addr & 0xFF; // Address byte 3	
	bufPtr[4] = 0xFE; // Nested byte
	
}

void fm_read_record(uint32_t addr, uint8_t * buffer){

//	uint8_t buf[64] = {0};

//	uint8_t data;

//	gpio_set_level(CS, 0);	// Stan niski na linii CS układu
//	HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, 0);
	rb_CsReset();
//	HAL_SPI_Transmit(&hspi1, &data, 1, 0);


	rb_SpiWriteByte(0x03);
	// uint32_t rw = rb_SpiWriteByte(0x03);

	
	rb_SpiWriteByte(addr>>16);
	// uint32_t a3 = rb_SpiWriteByte(addr>>16);

	
	rb_SpiWriteByte(addr>>8);
	// uint32_t a2 = rb_SpiWriteByte(addr>>8);

	
	rb_SpiWriteByte(addr);
	// uint32_t a1 = rb_SpiWriteByte(addr);


	rb_SpiWriteByte(0xFE); // Nested byte
	// uint32_t t1 = rb_SpiWriteByte(0xFE); // Nested byte
	// uint32_t t2 = rb_SpiWriteByte(0xFD); // Nested byte
	// uint32_t t3 = rb_SpiWriteByte(0xFC); // Nested byte
	// uint32_t t4 = rb_SpiWriteByte(0xFB); // Nested byte
	// uint32_t t5 = rb_SpiWriteByte(0xFA); // Nested byte
	// rb_SpiWriteByte(0x00); // Additional dummy byte for 64-byte read

	rb_SpiReadBuffer(buffer, 64);

	// for (uint32_t i = 0; i < 64; i++) {
	// 	buffer[i] = rb_SpiWriteByte(0x00);
	// 	// buffer[i] = spi1_transfer(0x00);
	// 	// rb_SpiWriteByte(0x00);
	// }

	rb_CsSet();
}
