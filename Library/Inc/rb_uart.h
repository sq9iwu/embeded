#ifndef _RB_UART_H
#define _RB_UART_H

#include <stdint.h>
#include "rb_common.h"

rb_status_t rb_uart_init(void);
rb_status_t rb_uart_put_char(const uint8_t data);
rb_status_t rb_uart_put_str(const uint8_t* data);
rb_status_t rb_uart_transmit(uint8_t *data, uint16_t size);

rb_status_t rb_set_uart_interrrupt_priority(uint32_t priority);
rb_status_t rb_uart_register_rx_callback(void (*callback)(uint8_t data));

#endif