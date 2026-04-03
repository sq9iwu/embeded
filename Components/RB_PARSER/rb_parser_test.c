/*
 * rb_parser.c
 *
 *  Created on: 14 mar 2026
 *      Author: Administrator
 */

#include <string.h>
#include <stdlib.h>


#include "rb_parser_test.h"

static void (*parser_callback)(void * pvParams) = NULL;
static void (*line_read_callback)(void * pvParams) = NULL;

test_data_t data_in;

char * test1;
char * test2;
char * test3;

char buffer[128];
int i = 0;

void rb_get_line(char c){
    if(c != '\r' && c != '\n') {
      buffer[i] = c; // Append received character to buffer
      i++;

    } else {
        if(i > 0) { // If we have received some data before the newline
            buffer[i + 1] = '\0'; // Null-terminate the string
            if(NULL != line_read_callback) line_read_callback(buffer); // Call line read callback with the complete line
            i = 0; // Reset buffer index for next line
        }
    }
}


void rb_parse_test(uint8_t * data, size_t len){

    strtok_r((char*)data, ";", &test1);

    if(!test1) return;	// Zabezpieczenie przed pustym - niew│aťciwym stringiem

    if(*test1 != 0){
    	strtok_r(test1, ";", &test2);
    }
    if(*test2 != 0){
    	strtok_r(test2, ";", &test3);
    }

//                printf("Data: %s\r\n", raw_data);

    data_in.command = atoi((char*)data);
//                printf("Test1: %s\r\n", test1);
    data_in.addr = atol(test1);

    data_in.len = atol(test2);
//                printf("Test2: %s\r\n", test2);


    for(uint8_t i = 0; i<strlen(test2); i++){
    	data_in.data[i] = (uint8_t)test2[i];
    }

    if(parser_callback) parser_callback(&data_in);

//    xStatus = xQueueSendToBack(xQincomingData, &data_in, 0);

}

void rb_parser_register_callback(void (*callback)(void * pvParams)){
	parser_callback = callback;
}

void rb_line_read_register_callback(void (*callback)(void * pvParams)){
    line_read_callback = callback;
}