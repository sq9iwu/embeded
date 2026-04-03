#ifndef _CMSIS_HELPERS
#define _CMSIS_HELPERS

#define FILL_REG(REG, BITS, VAL) {			\
	REG &= ~BITS;							\
	REG |= ((VAL << BITS##_Pos) & BITS);	\
}											\

#define GPIO_MODER_INPUT		0x00
#define GPIO_MODER_OUTPUT		0x01
#define GPIO_MODER_AF			0x02
#define GPIO_MODER_ANALOG		0x03

#define GPIO_OTYPER_PUSH_PULL	0x00
#define GPIO_OTYPER_OP_DRAIN 	0x01

#define GPIO_PUPDR_NONE			0x00
#define GPIO_PUPDR_PUP			0x01
#define GPIO_PUPDR_PDN			0x02

#define SYSCFG_EXTI_PORTA		0x00
#define SYSCFG_EXTI_PORTB		0x01
#define SYSCFG_EXTI_PORTC		0x02
#define SYSCFG_EXTI_PORTD		0x03
#define SYSCFG_EXTI_PORTE		0x04
#define SYSCFG_EXTI_PORTH		0x07

#define GPIO_AFR_AF0			0x00
#define GPIO_AFR_AF1			0x01
#define GPIO_AFR_AF2			0x02
#define GPIO_AFR_AF3			0x03
#define GPIO_AFR_AF4			0x04
#define GPIO_AFR_AF5			0x05
#define GPIO_AFR_AF6			0x06
#define GPIO_AFR_AF7			0x07
#define GPIO_AFR_AF8			0x08
#define GPIO_AFR_AF9			0x09
#define GPIO_AFR_AF10			0x10
#define GPIO_AFR_AF11			0x11
#define GPIO_AFR_AF12			0x12
#define GPIO_AFR_AF13			0x13
#define GPIO_AFR_AF14			0x14
#define GPIO_AFR_AF15			0x15



#endif
