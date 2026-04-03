/*
 * rb_spi.c
 *
 *  Created on: Mar 23, 2026
 *      Author: RICO
 */
#include "stm32f3xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "cmsis_helpers.h"

#include "rb_spi.h"

static void spiTask(void *argument);
QueueHandle_t spiTxQueue; // Queue for data to be sent via SPI
SemaphoreHandle_t spiMutex; // Mutex for SPI bus access

void rb_SpiInit(void){


	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	FILL_REG(GPIOA->MODER, GPIO_MODER_MODER5, GPIO_MODER_AF);		// SCK	HW
	FILL_REG(GPIOA->MODER, GPIO_MODER_MODER6, GPIO_MODER_AF);		// MISO	HW
	FILL_REG(GPIOA->MODER, GPIO_MODER_MODER7, GPIO_MODER_AF);		// MOSI HW
	FILL_REG(GPIOB->MODER, GPIO_MODER_MODER6, GPIO_MODER_OUTPUT);	// SC	SW // PB6 on F303

	FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL5, GPIO_AFR_AF5);		// Alternaative function SPI1, 0x00 default
	FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL6, GPIO_AFR_AF5);		// Alternaative function SPI1
	FILL_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL7, GPIO_AFR_AF5);		// Alternaative function SPI1
	GPIOB->ODR |= GPIO_ODR_6;	// CS high by default

//	FILL_REG(SPI1->CR1, SPI_CR1_BR, 0x03);	// 011, Baud rate control, prescaler 16
	FILL_REG(SPI1->CR1, SPI_CR1_BR, 0x02);	// 010, Baud rate control, prescaler 8
//	SPI1->CR1 |= SPI_CR1_BIDIMODE;	// Bidirectional mode
	SPI1->CR1 |= SPI_CR1_SSM;		// Software slave management
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR1 |= SPI_CR1_MSTR;		// Master configuration
	//	SPI1->CR1 |= SPI_CR1_LSBFIRST;
	FILL_REG(SPI1->CR2, SPI_CR2_DS, 0x07);	// 0111, Data size 8-bit
	SPI1->CR2 |= SPI_CR2_FRXTH;	// FIFO reception threshold, 8-bit data is received when RXNE flag is set

	
	SPI1->CR1 |= SPI_CR1_SPE;		// SPI Enable

	xTaskCreate(spiTask, "SPI Tx Task", 256, NULL, 1, NULL);
	spiTxQueue = xQueueCreate(5, sizeof(SpiTransfer_t)); // Queue for 5 transfers
	spiMutex = xSemaphoreCreateMutex(); // Mutex for SPI bus access
}

static void spiTask(void *argument) {
	// uint8_t dataToSend = 0x55; // Przykładowy bajt do wysłania
	while (1) {
		SpiTransfer_t transfer;
		if (xQueueReceive(spiTxQueue, &transfer, portMAX_DELAY) == pdPASS) {
			if (xSemaphoreTake(spiMutex, portMAX_DELAY) == pdPASS) {
				rb_SpiWriteBuffer(transfer.data, transfer.len);
				xSemaphoreGive(spiMutex);
			}
		}
		
	}
}

void rb_send_spi_data(uint8_t* data, uint32_t len) {
	SpiTransfer_t transfer = { .data = data, .len = len };
	xQueueSend(spiTxQueue, &transfer, portMAX_DELAY);
}

uint8_t rb_SpiWriteByte(uint8_t data){
	static uint8_t temp = 0;


	while(!(SPI1->SR & (SPI_SR_TXE))){}
	*(__IO uint8_t*)&SPI1->DR = data;
	// SPI1->DR = data;

	// while(!(SPI1->SR & (SPI_SR_TXE))){}

	// while(!(SPI1->SR & (SPI_SR_BSY))){}
	while (!(SPI1->SR & SPI_SR_RXNE)){}

	// temp = *(volatile uint8_t*)&SPI1->DR;
	// temp = *(__IO uint8_t*)&SPI1->DR;
	temp = SPI1->DR;

	// if (SPI1->SR & SPI_SR_OVR) 
	// {
	// 	SPI1->SR &= ~(SPI_SR_OVR); // Clear OVR flag
	// }
	// uint8_t dummy = SPI1->SR; // Clear OVF flag
	// (void)dummy; // Uniknięcie ostrzeżenia kompilatora "unused variable"
	// uint8_t b3 = (temp) >> 16;
	// uint8_t b2 = (temp) >> 8;
	// uint8_t b1 = (temp) >> 4;
	// uint8_t b0 = temp;
	
	
	return temp;

}
// Sztczuna entegolencjona

// void SPI1_ClearOverrun(void) {
//     volatile uint8_t temp;
//     temp = *(__IO uint8_t *)&SPI1->DR; // 1. Odczyt danych (wymuszone 8-bit)
//     temp = SPI1->SR;                   // 2. Odczyt statusu
//     (void)temp;                        // Uniknięcie ostrzeżenia kompilatora "unused variable"
// }

// uint8_t spi1_transfer(uint8_t data) {
//     // 1. Upewnij się, że TX FIFO ma miejsce (TXE - Transmit buffer empty)
//     while (!(SPI1->SR & SPI_SR_TXE));

//     // 2. Wymuś zapis 8-bitowy do DR (start transferu)
//     *(__IO uint8_t *)&SPI1->DR = data;

//     // 3. Czekaj na odbiór LUB błąd
//     while (!(SPI1->SR & SPI_SR_RXNE)) {
//         if (SPI1->SR & SPI_SR_OVR) {
//             SPI1_ClearOverrun();
//             return 0xFF; // Lub inna obsługa błędu
//         }
//     }

//     // 4. Odczytaj odebrany bajt (również wymuszone 8-bit)
//     return *(__IO uint8_t *)&SPI1->DR;
// }

// uint8_t spi1_transfer(uint8_t data) {


//     // Rzutowanie na __IO uint8_t* wymusza instrukcję STRB (8-bit store)
//     *(__IO uint8_t *)&SPI1->DR = data;

//     // Czekaj aż TX FIFO zwolni miejsce (opcjonalnie) i RX FIFO odbierze bajt
//     while (!(SPI1->SR & SPI_SR_RXNE));

//     // Rzutowanie na __IO uint8_t* wymusza instrukcję LDRB (8-bit load)
//     return *(__IO uint8_t *)&SPI1->DR;
// }

void rb_SpiWriteBuffer(uint8_t* data, uint32_t len){

	uint32_t i = 0;
	uint32_t temp = 0;

	uint8_t testBuf[69] = {0};

	rb_CsReset(); // Ustawienie CS nisko przed rozpoczęciem transmisji

	while(i<len){
		while(!(SPI1->SR & (SPI_SR_TXE))){}
		*(volatile uint8_t*)&SPI1->DR = data[i];

		while(!(SPI1->SR & (SPI_SR_RXNE))){}

		/* Clear OVF flag */	//????
		testBuf[i] = SPI1->DR; // Reading data
		uint32_t sr_temp = SPI1->SR; // Clear OVF flag
		(void)sr_temp; // Uniknięcie ostrzeżenia kompilatora "unused variable"

		i++;
	}

	// /* Wait for TXEmpty flag */
	// while(!(SPI1->SR & (SPI_SR_TXE))){}

	// /* Wait for clear busy flag */
	// while(!(SPI1->SR & (SPI_SR_BSY))){}

	// while(!(SPI1->SR & (SPI_SR_RXNE))){}

	/* Clear OVF flag */	//????
	// temp = SPI1->DR; // Reading data
	// temp = SPI1->SR; // ???

	rb_CsSet(); // Ustawienie CS wysoko po zakończeniu transmisji
}

void rb_SpiReadBuffer(uint8_t* data, uint32_t len){

	while(len){

		while(!(SPI1->SR & (SPI_SR_TXE))){}
		*(volatile uint8_t*)&SPI1->DR = 0;


		while(!(SPI1->SR & (SPI_SR_RXNE))){}

		/* Read data from register */

			/* Wait for clear busy flag */
		// while(!(SPI1->SR & (SPI_SR_BSY)))
		// {
		// 	if(1){

		// 	}
		// }

		*data++ = (SPI1->DR);
	    uint32_t temp = SPI1->SR; // Clear OVF flag
	    // (void)temp; // Uniknięcie ostrzeżenia kompilatora "unused variable"		
		len--;
	}
}

void rb_CsSet(void){
//	while(!(SPI1->SR & (SPI_SR_BSY))){}
	for (int i = 0; i < 45; i++) {}
	// Pętla opóźniająca cs, wyłączał przed zakończeniem wysyłki
	//  Trzeba coś z tym pokompinować */
	GPIOB->BSRR |= GPIO_BSRR_BS_6;
//	GPIOB->ODR |= GPIO_ODR_OD0;
}

void rb_CsReset(void){
	GPIOB->BSRR |= GPIO_BSRR_BR_6;
//	GPIOB->ODR &= ~GPIO_ODR_OD0;
}
