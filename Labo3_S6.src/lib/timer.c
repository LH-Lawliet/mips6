#include "timer.h"


/* timerX_isr
 *   timerX ISR (Interrupt Service Routine)
 */
static OnTick callback2 = NULL;
static OnTick callback3 = NULL;
static OnTick callback4 = NULL;
static OnTick callback5 = NULL;

void TIM2_IRQHandler() {
	if (callback2) callback2();
	_TIM2->SR &= ~0x1F;
}

void TIM3_IRQHandler() {
	if (callback3) callback3();
	_TIM3->SR &= ~0x1F;
}

void TIM4_IRQHandler() {
	if (callback4) callback4();
	_TIM4->SR &= ~0x1F;
}

void TIM5_IRQHandler() {
	if (callback5) callback5();
	_TIM5->SR &= ~0x1F;
}


/* timer_tick_init
 *   setup timer to call cb function periodically, each tick_ms
 */
int timer_tick_init(TIM_t *tmr, uint32_t tick_ms, OnTick cb)
{
	IRQn_t	 irqn;
	uint32_t irq_priority, clk;
	clk = sysclks.apb1_timer_freq;
	
	if (tmr == _TIM2) {
		// register callback function
        //  < A COMPLETER >
        callback2 = cb;

        irqn = TIM2_IRQn;
		
		// enable timer clocking
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
        //  < A COMPLETER >
        
	} else if (tmr == _TIM3) {
		// register callback function
		//  < A COMPLETER >
		callback3 = cb;
		irqn = TIM3_IRQn;
		
		// enable timer clocking
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
		//  < A COMPLETER >
	
	} else if (tmr == _TIM4) {
		// register callback function
		//  < A COMPLETER >
		callback4 = cb;
		irqn = TIM4_IRQn;
		
		// enable timer clocking
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
		//  < A COMPLETER >
	
	} else if (tmr == _TIM5) {
		// register callback function
		//  < A COMPLETER >
		callback5 = cb;
		irqn = TIM5_IRQn;
		
		// enable timer clocking
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; 
		//  < A COMPLETER >
	
	} else {
		return -1;
	}
	
	// clear pending interrupts
	//  < A COMPLETER >
	tmr->SR = 0;

	// set mode
	//  < A COMPLETER >
	tmr->CR1 = TIM_CR1_ARPE;
	tmr->DIER |= TIM_DIER_UIE;
	
	// set prescaler 100us
	//  < A COMPLETER >
	tmr->PSC = clk/10000 - 1; //100us
	
	// set period
	//  < A COMPLETER >
	tmr->ARR = tick_ms*10-1;
	
	if (cb) {
		//  < A COMPLETER >
		irq_priority = 6; //6 is exti1
		NVIC_SetPriority(irqn, irq_priority);
		NVIC_EnableIRQ(irqn);
	}
	
    return 0;
}

/* timer_tick_period
 *   change the tick_ms period
 */
int timer_tick_period(TIM_t *tmr, uint32_t tick_ms)
{
    // set period
	//  < A COMPLETER >
	tmr->ARR = tick_ms*10-1;
    
    // force update to reset counter and prescaler
	//  < A COMPLETER >
	tmr->EGR = 1;
}

/* timer_start
 *   reset & enable counting
 */
void timer_start(TIM_t *tmr)
{
	// force update to reset counter and prescaler
	//  < A COMPLETER >
	tmr->EGR = 1;
	
	// enable counting
	//  < A COMPLETER >
    tmr->CR1 |= 1;
}

/* timer_stop
 *   stop counting
 */
void timer_stop(TIM_t *tmr)
{
	// disable counting
	//  < A COMPLETER >
	tmr->CR1 &= ~1;
}

