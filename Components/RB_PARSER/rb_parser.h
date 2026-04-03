/*
 * iwu_parser.h
 *
 *  Created on: 4 lut 2024
 *      Author: Administrator
 */

#ifndef COMPONENTS_IWU_PARSER_IWU_PARSER_H_
#define COMPONENTS_IWU_PARSER_IWU_PARSER_H_

typedef struct{
	char * command;
	char * param1;
	char * param2;
	char * param3;
}at_command_t;

//typedef struct{
//	char name[32];
//	char value[20];
//}param_t;
//
//typedef struct{
//	char command[32];
//	param_t params[20];
//}json_command_t;

typedef struct{
	char name[32];
	char value[20];
}param_t;

typedef struct{
	char command[32];
	param_t params[20];
}json_command_t;

typedef void(*TAT_PARSER_CALLBACK)(at_command_t * cmd);
typedef void(*TJSON_PARSER_CALLBACK)(json_command_t * cmd);

extern void register_at_command_callback(TAT_PARSER_CALLBACK command_cb);
extern void register_json_command_callback(TJSON_PARSER_CALLBACK json_command_cb);

extern void at_parse_data(char *data);
extern void json_parse_data(char *data);

#endif /* COMPONENTS_IWU_PARSER_IWU_PARSER_H_ */
