/*
 * rb_parser.h
 *
 *  Created on: 14 mar 2026
 *      Author: Administrator
 */

#ifndef COMPONENTS_RB_PARSER_RB_PARSER_H_
#define COMPONENTS_RB_PARSER_RB_PARSER_H_

#include <stdint.h>
#include <stddef.h>

typedef struct{
	uint8_t command;
	uint32_t addr;
	uint32_t len;
	uint8_t data[64];
}test_data_t;

void rb_parse_test(uint8_t * data, size_t len);
void rb_get_line(char c);

void rb_parser_register_callback(void (*callback)(void * pvParams));
void rb_line_read_register_callback(void (*callback)(void * pvParams));




#endif /* COMPONENTS_RB_PARSER_RB_PARSER_H_ */
