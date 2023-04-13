#include <stdlib.h>
#include <stdarg.h>
#include "uart.h"
#include "io.h"
#include "util.h"

// USART Pin config
#define USART_PIN_CONFIG PIN_MODE_ALTFUNC|PIN_OPT_OUTPUT_PUSHPULL|PIN_OPT_OUTPUT_SPEED_LOW|PIN_OPT_RESISTOR_NONE|PIN_OPT_AF7// A COMPLETER

                             
#ifdef USE_USART1
static OnUartRx usart1_cb = 0;

void USART1_IRQHandler()
{
	uint32_t sr = _USART1->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<1)) || (sr & (1<<2)))) {
			if (usart1_cb) usart1_cb(_USART1->DR);
		} else {				// Noise or framing error or break detected
			_USART1->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART1->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART1->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART1->DR;
	}
}
#endif

#ifdef USE_USART2
static OnUartRx usart2_cb = 0;

void USART2_IRQHandler()
{
	uint32_t sr = _USART2->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<1)) || (sr & (1<<2)))) {
			if (usart2_cb) usart2_cb(_USART2->DR);
		} else {				// Noise or framing error or break detected
			_USART2->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART2->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART2->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART2->DR;
	}
}
#endif

#ifdef USE_USART6
static OnUartRx usart6_cb = 0;

void USART6_IRQHandler()
{
	uint32_t sr = _USART6->SR;
	
	if (sr & (1<<5)) {			// Read data register not empty interrupt
		if (!((sr & (1<<1)) || (sr & (1<<2)))) {
			if (usart6_cb) usart6_cb(_USART6->DR);
		} else {				// Noise or framing error or break detected
			_USART6->DR;
		}
	} else if (sr & (1<<4)) {	// idle interrupt
		_USART6->DR;
	} else if (sr & (1<<3)) {	// overrun interrupt
		_USART6->DR;
	} else if (sr & (1<<0)) {	// parity error interrupt
		_USART6->DR;
	}
}
#endif



/*
 * uart_init : polling Tx and IRQ Rx
 */
int uart_init(USART_t *u, uint32_t baud, uint32_t mode, OnUartRx cb)
{
	IRQn_t	irq_number;
	uint32_t irq_priority;
	
	if (u == _USART1) {
#ifdef USE_USART1
		// enable USART clocking
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

		// configure Tx/Rx pins : Tx -->, Rx --> 
		io_configure(USART1_GPIO_PORT, USART1_GPIO_PINS, USART_PIN_CONFIG, NULL);
		
        //baud rate
        int clk = sysclks.apb2_freq;
        int USARTDIV = clk/baud;
        u->BRR = USARTDIV;

        irq_number = USART1_IRQn;
        
        usart1_cb = cb;
#else
		return -1;
#endif
	 } else if (u == _USART2) {
#ifdef USE_USART2
		// enable USART clocking
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
		// configure Tx/Rx pins : Tx --> PA2, Rx --> PA3
		io_configure(USART2_GPIO_PORT, USART2_GPIO_PINS, USART_PIN_CONFIG, NULL);
		
        //baud rate
        int clk = sysclks.apb1_freq;
        int USARTDIV = clk/baud;
        u->BRR = USARTDIV;

        irq_number = USART2_IRQn;

        usart2_cb = cb;
#else
		return -1;
#endif
	} else if (u == _USART6) {
#ifdef USE_USART6
		// enable USART clocking
		RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
		
		// configure Tx/Rx pins
		io_configure(USART6_GPIO_PORT, USART6_GPIO_PINS, USART_PIN_CONFIG, NULL);

        //baud rate
        int clk = sysclks.apb2_freq;
        int USARTDIV = clk/baud;
        u->BRR = USARTDIV;

        usart6_cb = cb;
#else
		return -1;
#endif
	} else {
		return -1;
	}
	
	//USART configuration
    u->CR1 = ((mode & 0x1)<<12) | ((mode>>8 & 0x7)<<8) | 1<<3 | 1<<2 | 1<<13 ;
    u->CR2 = ((mode>>4 & 0x3)<<12);
    u->CR3 = 0;

	// Setup NVIC
	if (cb) {
		irq_priority = 6;
		NVIC_SetPriority(irq_number, irq_priority);
		NVIC_EnableIRQ(irq_number);
		u->CR1 |= 1<<5;
	}
	
    return 1;
}

/*
 * uart_getc : get a char from the serial link (blocking)
 */
char uart_getc(USART_t *u)
{
	while ((u->SR & 1<<5) == 0)
	{
	}
	char c = u->DR;
	return c;}

/*
 * uart_putc : send a char over the serial link (polling)
 */
void uart_putc(USART_t *u, char c)
{
  	while ((u->SR&(1<<7))==0) {}
  	u->DR = c;
  	while ((u->SR&(1<<6))==1) {}
}

/*
 * uart_puts : send a string over the serial link (polling)
 */
void uart_puts(USART_t *u, char *s)
{
	int i = 0;
	while (s[i] != '\0') 
	{
		uart_putc(u, s[i]);
  		i+=1;
	}
}

/*
 * uart_printf : print formatted text to serial link
 */
void uart_printf(USART_t * u, const char* fmt, ...)
{
	__gnuc_va_list        ap;
	char          *p;
	char           ch;
	unsigned long  ul;
	char           s[34];
	
	va_start(ap, fmt);
	while (*fmt != '\0') {
		if (*fmt =='%') {
			switch (*++fmt) {
				case '%':
					uart_putc(u,'%');
					break;
				case 'c':
					ch = va_arg(ap, char);
					// A COMPLETER
					uart_putc(u, ch);
					break;
				case 's':
					p = va_arg(ap, char *);
					uart_puts(u, p);
					// A COMPLETER
					break;
				case 'd':
					ch = va_arg(ap, int);
					// A COMPLETER
					num2str(s,ch,10);
					uart_puts(u, s);
					break;
				case 'u':
					// A COMPLETER
					ch = va_arg(ap, unsigned int);
					uart_puts(u, s);
					break;
				case 'x':
					// A COMPLETER
					ch = va_arg(ap, int);
					num2str(s,ch,16);
					uart_puts(u, s);
					break;
				default:
				    uart_putc(u, *fmt);
			}
		} else uart_putc(u, *fmt);
		fmt++;
	}
	va_end(ap);
}
