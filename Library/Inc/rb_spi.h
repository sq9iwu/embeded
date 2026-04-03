/*
 * rb_spi.h
 *
 *  Created on: Mar 23, 2026
 *      Author: RICO
 */

#ifndef RB_SPI_H_
#define RB_SPI_H_

#include <stdint.h>

typedef struct {
    uint8_t* data;
    uint32_t len;
} SpiTransfer_t;

// extern QueueHandle_t spiTxQueue; // Queue for data to be sent via SPI

void rb_SpiInit(void);
uint8_t rb_SpiWriteByte(uint8_t data);
void rb_SpiWriteBuffer(uint8_t* data, uint32_t len);
void rb_SpiReadBuffer(uint8_t* data, uint32_t len);
void rb_CsSet(void);
void rb_CsReset(void);

void rb_send_spi_data(uint8_t* data, uint32_t len);

// uint8_t spi1_transfer(uint8_t data) ;

#endif /* RB_SPI_H_ */
