#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "queue.h"  
#include "stm32f3xx.h"

#include "rb_uart.h"
#include "cmsis_helpers.h"


QueueHandle_t uartTxQueue;
QueueHandle_t uartRxQueue;

static void (*uartRxCallback)(uint8_t data) = NULL;

// static void uart_tx_Task(void *argument);
// static void uart_rx_Task(void *argument);

rb_status_t rb_uart_init(void)
{
    /* Turn on GPIOA clock */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;  

    /* Turn on USART2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure PA2 and PA3 for USART2 (AF7) */
    // GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    // GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
    FILL_REG(GPIOA->MODER, GPIO_MODER_MODER2, GPIO_MODER_AF);
    FILL_REG(GPIOA->MODER, GPIO_MODER_MODER3, GPIO_MODER_AF);
    FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL2, GPIO_AFR_AF7);
    FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL3, GPIO_AFR_AF7);

    /* Configure USART2: 115200 baud */
    uint32_t uart_div = (CONFIG_UART_BUS_CLK + CONFIG_UART_BAUD_RATE / 2) / CONFIG_UART_BAUD_RATE; // Assuming APB1 clock is 48 MHz
    USART2->BRR = uart_div;
    /* Enable Transmitter and Receiver */
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE;
    /* Enable USART */
    USART2->CR1 |= USART_CR1_UE;

    /* Uart rx interrupt enable */
    USART2->CR1 |= USART_CR1_RXNEIE;

    /* Set default interrupt priority */
    NVIC_SetPriority(USART2_IRQn, 5);
    NVIC_EnableIRQ(USART2_IRQn);

    uartTxQueue = xQueueCreate(10, sizeof(uint8_t));
    uartRxQueue = xQueueCreate(10, sizeof(uint8_t));

    // xTaskCreate(uart_tx_Task, "UART_TX", 128, NULL, 1, NULL);
    // xTaskCreate(uart_rx_Task, "UART_RX", 128, NULL, 1, NULL);

    return RB_OK;
}

rb_status_t rb_uart_put_char(const uint8_t data){
    // xQueueSend(uartTxQueue, &data, pdMS_TO_TICKS(50));
    while (!(USART2->ISR & USART_ISR_TXE)); // Wait until transmit data register is empty
    USART2->TDR = (data & 0xFFU); // Load byte to transmit
    return RB_OK;
}

rb_status_t rb_uart_put_str(const uint8_t* data){
    while (*data) {
        rb_uart_put_char(*data++);
    }
    return RB_OK;
}




rb_status_t rb_uart_transmit(uint8_t *data, uint16_t size)
{
    USART2->TDR = *data; // Load first byte to transmit
    for (uint16_t i = 1; i < size; i++) {
        while (!(USART2->ISR & USART_ISR_TXE)); // Wait until transmit data register is empty
        USART2->TDR = data[i]; // Load next byte to transmit
    }
    return RB_OK;
}

// static void uart_tx_Task(void *argument)
// {
//     uint8_t txData;
//     while (1) {

//         if (xQueueReceive(uartTxQueue, &txData, pdMS_TO_TICKS(50)) == pdPASS) {
//             rb_uart_put_char(txData);
//         }
//     }
// }

// static void uart_rx_Task(void *argument)
// {
//     uint8_t rxData;
//     while (1) {
//         if (USART2->ISR & USART_ISR_RXNE) { // Check if data is received
//             rxData = USART2->RDR; // Read received byte
//             xQueueSend(uartRxQueue, &rxData, portMAX_DELAY); // Send to RX queue
//         }
//         vTaskDelay(pdMS_TO_TICKS(10)); // Small delay to prevent task hogging CPU
//     }
// }

rb_status_t rb_set_uart_interrrupt_priority(uint32_t priority){
    NVIC_SetPriority(USART2_IRQn, priority);
    return RB_OK;
}

rb_status_t rb_uart_register_rx_callback(void (*callback)(uint8_t data)){
    uartRxCallback = callback;
    return RB_OK;
}

int __io_putchar(int ch)
{
    rb_uart_put_char((uint8_t)ch);
    return ch;
}

void USART2_IRQHandler(void)
{
    if (USART2->ISR & USART_ISR_RXNE) { // Check if data is received
        uint8_t rxData = USART2->RDR; // Read received byte
        if (uartRxCallback) {
            uartRxCallback(rxData); // Call registered callback with received data
        }
    }
}