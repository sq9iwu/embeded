/*
 * iwu_parser.c
 *
 *  Created on: 4 lut 2024
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_spi_flash.h"

// #include "driver/gpio.h"

// #include "portmacro.h"
// #include "FreeRTOSConfig.h"
// //#include "../build/include/sdkconfig.h"
// #include "tcpip_adapter.h"


#include "rb_parser.h"

at_command_t at_command;
json_command_t json_command;

TAT_PARSER_CALLBACK parser_cb;
TJSON_PARSER_CALLBACK json_parser_callback;

void register_at_command_callback(TAT_PARSER_CALLBACK command_cb){
	parser_cb = command_cb;
}

void register_json_command_callback(TJSON_PARSER_CALLBACK json_command_cb){
	json_parser_callback = json_command_cb;
}

void json_parse_data(char *data){

//    char str[] = "zigbee2mqtt/Balkon_Lux;{\"Hello\":World, \"Geeks\":\"for\", Dupa:Maryna, Jadzia:daje, hosts:[{name:RICARDO,ip:192.168.0.222,gw:255.255.255.0},{name:Ravis,ip:192.168.8.222,gw:255.255.255.0}]}";
//    char str[] = "zigbee2mqtt/Balkon_Lux;{\"Hello\":World, \"Geeks\":\"for\", Dupa:Maryna, Jadzia:daje}";
//    char str[] = "zigbee2mqtt/Pir_Serwis;{\"battery\":100,\"device_temperature\":26,\"illuminance\":4,\"illuminance_lux\":4,\"linkquality\":55,\"occupancy\":false,\"power_outage_count\":208,\"voltage\":3025}";
    memset(&json_command,0 ,sizeof(json_command_t));

    const char outer_delimiters[] = "{}[];\n";
    const char inner_delimiters[] = ": ,\"";

    char* token;
    char* outer_saveptr = NULL;
    char* inner_saveptr = NULL;
    uint8_t outer_count = 0;
    uint8_t inner_count = 0;
    uint8_t param_index = 0;

    token = strtok_r(data, outer_delimiters, &outer_saveptr);

    while (token != NULL) {

//        printf("Outer Token: %s      count:%d\n", token, outer_count);


        char* inner_token = strtok_r(token, inner_delimiters, &inner_saveptr);

        while (inner_token != NULL) {
//            printf("Inner Token: %s      count:%d\n", inner_token, inner_count);

            if(!inner_count) strcpy(json_command.command,token);

            if( inner_count && (inner_count % 2 != 0)) {
//            	printf("CNT:%d, NAME:%s\n",inner_count, inner_token);
            	strcpy(json_command.params[param_index].name, inner_token);
            }
            if( inner_count && (inner_count % 2 == 0)) {
//            	printf("CNT:%d, PAR:%s\n",inner_count, inner_token);
            	strcpy(json_command.params[param_index].value, inner_token);
//            	printf("Index: %d\n", param_index);
            	param_index++;
            }
//            if(inner_count && inner_count % 2 == 0) strcpy(json_command.params[inner_count].value, inner_token);

            inner_token = strtok_r(NULL, inner_delimiters, &inner_saveptr);
            inner_count++;
        }

        token = strtok_r(NULL, outer_delimiters, &outer_saveptr);
        outer_count++;
    }

    if(json_parser_callback) json_parser_callback(&json_command);

//    for(uint8_t i = 0; i < 20; i++){
//    	if(strlen(json_command.params[i].name)){
//            printf("[%d] Command: %s, ParamName: %s, Value: %s\n", i, json_command.command,json_command.params[i].name,json_command.params[i].value);
//    	}
//    }



//    printf("Command: %s\n", json_command.command);
//    return 0;
//	char *reszta;
//
//	char * td = strtok_r((char*)data, "{}", &reszta);
//
//	if(td){
//			printf("Reszta: %s\n", td);
//	}else{
////			printf("Reszta is NULL\n");
//	}
//
//	if(reszta){
//			printf("Reszta: %s\n", reszta);
//	}else{
////			printf("Reszta is NULL\n");
//	}

}


void at_parse_data(char *data){

//	"+CMD*TIME:%02d,min:%02d,nday:%03s,day:%02d,month:%02d\r\n"


	uint8_t error = 0;
	char prefix[2]={0};

	char *reszta, *cmd, *str, *data2=NULL,*par1=NULL,*par2=NULL,*par3=NULL;
//	char *str, *reszta, *val1, *val2, *val3, *val4, *val5, *par1, *par2, *par3, *par4, *par5;


	strncpy(prefix,(char*)data,2);

	if( strncmp( (char*)data, "AT+", 3 ) == 0 ){

		strtok_r((char*)data, "+", &reszta);

		cmd = strtok_r(reszta,"?", &str);
//		at_command.command = cmd;

		data2 = strtok_r(str,"=", &reszta);

		par1 = strtok_r(reszta,",", &str);

		par2 = strtok_r(str,",", &reszta);

		par3 = strtok_r(reszta,",", &str);

		if(cmd){
//			printf("Command: %s\r\n", cmd);
			at_command.command = cmd;
			at_command.param1 = "";
			at_command.param2 = "";
			at_command.param3 = "";
		}else{
//			printf("Command is NULL\r\n");
		}

		if(cmd){
//			printf("cmd: %s\n", cmd);
//			at_command.param = reszta;
		}else{
//			printf("cmd is NULL\n");
		}
		if(reszta){
//			printf("Reszta: %s\n", reszta);
//			at_command.param = reszta;
		}else{
//			printf("Reszta is NULL\n");
		}

		if(str){
//			printf("Str: %s\n", str);
		}else{
//			printf("Str: is NULL\n");
		}
		if(data2){
//			printf("Data2: %s\n", data2);
		}else{
//			printf("Data2 is NULL\n");
		}
		if(par1){
			at_command.param1 = par1;
//			printf("Par1: %s\n", at_command.param1);
		}else{
//			printf("Par1 is NULL\n");
		}
		if(par2){
			at_command.param2 = par2;
//			printf("par2: %s\n", at_command.param2);
		}else{
//			printf("par2 is NULL\n");
		}
		if(par3){
			at_command.param3 = par3;
//			printf("par3: %s\n", at_command.param3);
		}else{
//			printf("par3 is NULL\n");
		}
		if(parser_cb) parser_cb(&at_command);



//
//		if(!strncmp(par1,"TIME",4)){
//			val1 = strtok_r(str,",", &reszta);	// Godziny
//			par2 = strtok_r(reszta,":", &str);
//			val2 = strtok_r(str,",", &reszta);	// Minuty
//			par3 = strtok_r(reszta,":", &str);
//			val3 = strtok_r(str,",", &reszta);	// Dzień nazwa
//			par4 = strtok_r(reszta,":", &str);
//			val4 = strtok_r(str,",", &reszta);	// Dzień cyfra
//			par5 = strtok_r(reszta,":", &str);
//			val5 = strtok_r(str,",", &reszta);	// Miesiąc
//
//
//		}
//		if(!strncmp(par1,"LUX",3)){
//			val1 = strtok_r(str,",", &reszta);	// LUX
//		}

//		val1 = strtok_r(str,",", &reszta);
//
//		par2 = strtok_r(reszta,":", &str);
//		val2 = strtok_r(str,",", &reszta);
//
//		par3 = strtok_r(reszta,":", &str);
//		val3 = strtok_r(str,",", &reszta);
//
//		par4 = strtok_r(reszta,":", &str);
//		val4 = strtok_r(str,",", &reszta);
//
//		par5 = strtok_r(reszta,":", &str);
//		val5 = strtok_r(str,",", &reszta);

	}
	if( strncmp( prefix, "AT", 2 ) == 0 && !error ){
		printf("OK\r\n");
		bzero(prefix,sizeof(prefix));
	}

}
