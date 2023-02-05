#include "timer.h"

#include "libshield/lcd_128x32.h"

/* timer_wait_ms
 *   wait for ms millisecond function
 */
int timer_wait_ms(TIM_t *tmr, uint32_t ms, OnTick cb)
{
//	< A COMPLETER >

}


/* timer_wait_us
 *   wait for us microsecoonds function
 */
int timer_wait_us(TIM_t *tmr, uint32_t us, OnTick cb)
{
    //	< A COMPLETER >
    tmr->CCER = 0;
    tmr->SMCR = 0;
    tmr->PSC=249;
    tmr->ARR=63999;
    tmr->CNT=0;
    tmr->CR1=1;
    for (uint32_t i = 0; i<1000; ++i) {
        cls();
        lcd_printf("oui oui %d %d\n", tmr->CNT, tmr->SR);
        //lcd_printf("oui : %d \n", TIM2->SR);
    }
    return 0;
    //
}


/* timer_count_init
 *   setup timer to count each timebase_us us
 */
int timer_count_init(TIM_t *tmr, uint32_t timebase_us)
{
    
//	< A COMPLETER >

}


/* timer_start
 *   reset & enable counting
 */
void timer_start(TIM_t *tmr)
{
    
//	< A COMPLETER >

}

/* timer_stop
 *   stop counting
 */
void timer_stop(TIM_t *tmr)
{
    
//	< A COMPLETER >

}

/* read_timer
 *   return the counter value
 */
uint32_t read_timer(TIM_t *tmr)
{
    
//	< A COMPLETER >

}

