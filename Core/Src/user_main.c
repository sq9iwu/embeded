#include "FreeRTOS.h"
// #include "gpio.h"
#include "portmacro.h"
#include "projdefs.h"
#include "task.h" 
#include "queue.h"
#include <stdio.h>

#include "rb_spi.h" // Do wywalenia potem
#include "fm_reader.h"
#include "rb_uart.h"

#include "rb_parser_test.h"

uint32_t loopCnt = 512;
uint8_t buf[64] = {0};
static volatile QueueHandle_t rxQueue; // Queue for received data from UART

void parser_callback(void * pvParams){

	test_data_t * parse_data = (test_data_t*)pvParams;

	switch (parse_data->command) {

		case GET_INFO:
			printf("STM32F303-IF v1.2\r\n");
			break;

        case GET_DRECORD:
//        	ReadRepairedRecords(repaddr);
//        	readSRecord(data_ready.addr);
            
            fm_read_record(parse_data->addr, buf);
//        	address = data_ready.addr;
            break;

        case GET_RCRC:
        	// ReadBytesAndCrc(parse_data->addr, parse_data->len);
            break;

		case READ_FULL_INFO:
			// fm_get_full_info();
			break;
	}

}

void line_read_callback(void * pvParams){
    char * line = (char *)pvParams;
    // printf("Received line: %s\n", line); // Print the complete line received from UART
    rb_parse_test((uint8_t *)line, 1); // Argument '1' is a placeholder, adjust as needed for actual line length
}

void ReadTask(void *argument) {

    // char buffer[128];
    while (1)
    {
        uint8_t c; 
        xQueueReceive(rxQueue, &c, portMAX_DELAY); // Receive data from UART RX queue
        rb_get_line(c); // Process received character to build lines
    }
}

void TestSend(void *argument){

  vTaskDelay(pdMS_TO_TICKS(800));
  
  while (1)
  {
    uint8_t buf[69] = {0};
    buf[0] = 0x03; // Read command
    buf[1] = (loopCnt >> 16) & 0xFF; // Address byte 1
    buf[2] = (loopCnt >> 8) & 0xFF; // Address byte 2
    buf[3] = loopCnt & 0xFF; // Address byte 3  
    buf[4] = 0x00; // Nested byte
    rb_send_spi_data(buf, sizeof(buf)); // Send command and address over SPI
		// fm_read_record(loopCnt, buf);
//		rb_CsSet();
//		rb_SpiWriteByte(0x09);
//		rb_CsReset();
		// for (int i = 0; i < 600000; i++) {}
    vTaskDelay(pdMS_TO_TICKS(800));
		loopCnt += 64;

    // rb_uart_put_char(loopCnt & 0xFFU); // Send least significant byte of loopCnt over UART
    printf("Read record at address: %lu\n", loopCnt); // Print the current address being read
    if(loopCnt >= 130944) loopCnt = 512;
    // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);  
    // vTaskDelay(500);
  }
  
}

// ************************************** 

void uart_data_received_callback(uint8_t data) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(rxQueue, &data, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void user_main_init(void){

    rb_SpiInit();
    rb_uart_init();

    xTaskCreate(TestSend, "TestSend", 256, NULL, 1, NULL);
    xTaskCreate(ReadTask, "ReadTask", 256, NULL, 1, NULL);
    rb_uart_register_rx_callback(uart_data_received_callback);

    rb_parser_register_callback(parser_callback);
    rb_line_read_register_callback(line_read_callback);

    rxQueue = xQueueCreate(10, sizeof(uint8_t)); // Create a queue to hold received bytes from UART
}

