#ifndef _RB_COMMON_H
#define _RB_COMMON_H

typedef enum {
    RB_OK = 0x00U,
    RB_ERROR = 0xFFU,
    RB_BUSY = 0xFEU,
    RB_TIMEOUT = 0xFDU
} rb_status_t;

#define CONFIG_UART_BUS_CLK 24000000U
#define CONFIG_UART_BAUD_RATE 115200U

#endif