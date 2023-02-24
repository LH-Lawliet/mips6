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
	
	if (tmr == _TIM2) {
		// register callback function
        //  < A COMPLETER >
		
		// enable timer clocking
        //  < A COMPLETER >
        
	} else if (tmr == _TIM3) {
		// register callback function
		//  < A COMPLETER >
		
		// enable timer clocking
		//  < A COMPLETER >
	
	} else if (tmr == _TIM4) {
		// register callback function
		//  < A COMPLETER >
		
		// enable timer clocking
		//  < A COMPLETER >
	
	} else if (tmr == _TIM5) {
		// register callback function
		//  < A COMPLETER >
		
		// enable timer clocking
		//  < A COMPLETER >
	
	} else {
		return -1;
	}
	
	// clear pending interrupts
	//  < A COMPLETER >
	
	// set mode
	//  < A COMPLETER >
	
	// set prescaler 100us
	//  < A COMPLETER >
	
	// set period
	//  < A COMPLETER >
	
	if (cb) {
		//  < A COMPLETER >
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
    
    // force update to reset counter and prescaler
	//  < A COMPLETER >
}

/* timer_start
 *   reset & enable counting
 */
void timer_start(TIM_t *tmr)
{
	// force update to reset counter and prescaler
	//  < A COMPLETER >
	
	// enable counting
	//  < A COMPLETER >
}

/* timer_stop
 *   stop counting
 */
void timer_stop(TIM_t *tmr)
{
	// disable counting
	//  < A COMPLETER >
}

