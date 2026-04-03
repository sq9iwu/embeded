/*
 * fm_reader.h
 *
 *  Created on: Mar 22, 2026
 *      Author: Administrator
 */

#ifndef FM_READER_H_
#define FM_READER_H_

#include <stdint.h>

typedef enum{
	WRITE = 2U,
	READ = 3U
}fm_mode_t;

typedef struct 
{
	fm_mode_t mode;
	uint32_t addr;
	uint32_t len;
	uint8_t * data;
}fm_data_t;


enum {
	GET_INFO = 1U,
	GET_XRECORD = 2U,
	GET_SRECORD = 3U,
	GET_DRECORD = 4U,
	GET_RCRC = 5U,
	LED_ON = 20U,
	LED_OFF = 30U,
	READ_FULL_INFO = 40,
	ACK = 50U,
	WRITE_INIT = 55U,
	WRITE_RECORD = 65U
};


void fm_read_record(uint32_t addr, uint8_t * buffer);

#endif /* FM_READER_H_ */
