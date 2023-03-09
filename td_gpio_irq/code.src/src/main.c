#include <stdlib.h>
#include "include/board.h"

#define SYSCFG_EXTI_PA_MASK		0
#define SYSCFG_EXTI_PB_MASK		1
#define SYSCFG_EXTI_PC_MASK		2
#define SYSCFG_EXTI_PD_MASK		3
#define SYSCFG_EXTI_PE_MASK		4
#define SYSCFG_EXTI_PH_MASK		7

/* 
 *  blue_led(uint32_t on)
 *    on=1 ==> led on, on=0 ==> led off
 */
void blue_led(uint32_t on) {
	if (on) {
		_GPIOA->BSRR |= 1<<(9+16); 
	} else {
		_GPIOA->BSRR |= 1<<9; 
	}
}

volatile int on=1;
int cpt=0;

// External Interrupt Service Routine
void EXTI9_5_IRQHandler() {
	/* A COMPLETER */
	on = !on;
	blue_led(on);

	_EXTI->PR |= 1<<5; 
}

int main()
{
	/*************************** GPIO Config ******************************/
	// Enable clock for GPIOA, GPIOB to be able to use it
	/* A COMPLETER */
	_RCC->AHB1ENR |= 1;
	_RCC->AHB1ENR |= 1<<1;
	
	// Configure input SW_CENTER (PB5)
	/* A COMPLETER */
	_GPIOB->MODER &= ~(3<<10);
	_GPIOB->PUPDR &= ~(3<<10);
	
	// Configure output LED_BLUE (PA9)
	/* A COMPLETER */
	_GPIOA->MODER &= ~(1<<19);
	_GPIOA->MODER |= (1<<18);
	_GPIOA->OTYPER &= ~(1<<9);
	_GPIOA->OSPEEDR &= ~(1<<19);
	_GPIOA->OSPEEDR |= (1<<18);
	_GPIOA->PUPDR &= ~(3<<18);
	
	blue_led(on);
	
	/*************************** EXTI Config ******************************/
	// enable clock for SYSCFG, no need for EXTI (clock never switched off)
	/* A COMPLETER */
	RCC->APB2ENR |= 1<<14;
	
	// configure pin PB5 (4 pin config per EXTICR[] register, 4 bits per pin)
	//   PB5 --> EXTI5
	/* A COMPLETER */
	_SYSCFG->EXTICR[1] &= ~(SYSCFG_EXTICR2_EXTI5_PB);
	_SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB;
	
	// allow pin EXTI5 to send an IRQ
	/* A COMPLETER */
	_EXTI->IMR |= 1<<5;

	// not a wakeup event
	/* A COMPLETER */
	_EXTI->EMR &= ~(1<<5);
	
	// Configure pin event IRQ on rising (RTSR)/falling (FTSR) edge (rising only here)
	/* A COMPLETER */
	_EXTI->RTSR |= 1<<5;
	_EXTI->FTSR &= ~(1<<5);
	
	// reset any pending IRQ on PB5
	/* A COMPLETER */
	_EXTI->PR |= 1<<5;
	
	/*************************** NVIC Config ******************************/
	// Vector Table ISR         : EXTI9_5_IRQHandler
	// IRQ Number (hard coded)  : EXTI9_5_IRQn = 23
	// Choose Priority          : 6
	
	// Set priority : 1 byte per NVIC IRQ input
	/* A COMPLETER */
	_NVIC->IP[23] = 6<<4;
	
	// Enable IRQ Input
	/* A COMPLETER */
	_NVIC->ISER[0] |= 1<<23;
	
	while (1) {
		/* wait for IRQ */
	}
	
	return 0;
}