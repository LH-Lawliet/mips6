#include "timer.h"

/* timer_wait_ms
 *   wait for ms millisecond function
 */
int timer_wait_ms(TIM_t *tmr, uint32_t ms, OnTick cb)
{
//	< A COMPLETER >
    timer_wait_us(tmr, ms*1000, cb); //c'est bancale vaudrait mieux refaire
}


/* timer_wait_us
 *   wait for us microsecoonds function
 */
int timer_wait_us(TIM_t *tmr, uint32_t us, OnTick cb)
{
    //	< A COMPLETER >

    uint32_t clk = sysclks.apb1_timer_freq;
    if (tmr == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
    }
    else if (tmr == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
        
    }
    else if (tmr == TIM4)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
        
    }
    else if (tmr == TIM5)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; 
    }

    uint32_t mask = (TIM_CR1_CEN|TIM_CR1_UDIS|TIM_CR1_URS|TIM_CR1_OPM|TIM_CR1_DIR|TIM_CR1_CMS_1|TIM_CR1_CKD|0xFC00);
    tmr->CR1 = (tmr->CR1 & mask) | TIM_CR1_OPM | TIM_CR1_ARPE;
    tmr->PSC = clk/1000000 - 1; 
    tmr->ARR = us-1;
    tmr->EGR = 1;
    tmr->CR1 |= 1;
    while (tmr->CR1 & 1)
    {
    }
    return 0;
}


/* timer_count_init
 *   setup timer to count each timebase_us us
 */
int timer_count_init(TIM_t *tmr, uint32_t timebase_us)
{
    
//	< A COMPLETER >
    uint32_t clk = sysclks.apb1_timer_freq;
    if (tmr == TIM2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
        tmr->ARR = 1<<33-2;
    }
    else if (tmr == TIM3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
        tmr->ARR = 1<<17-2;
        
    }
    else if (tmr == TIM4)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; 
        tmr->ARR = 1<<17-2;
        
    }
    else if (tmr == TIM5)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; 
        tmr->ARR = 1<<33-2;
    }

    tmr->CR1 = (TIM_CR1_CEN|TIM_CR1_DIR|TIM_CR1_CMS_1|TIM_CR1_CKD|0xFC00);
    tmr->PSC = clk/10000 - 1; 
    tmr->EGR = 1;
}


/* timer_start
 *   reset & enable counting
 */
void timer_start(TIM_t *tmr)
{
//	< A COMPLETER >
    tmr->CNT = 0;
    tmr->CR1 |= 1;
}

/* timer_stop
 *   stop counting
 */
void timer_stop(TIM_t *tmr)
{
    
//	< A COMPLETER >
    tmr->CR1 &= 2<<16-1;

}

/* read_timer
 *   return the counter value
 */
uint32_t read_timer(TIM_t *tmr)
{
    
//	< A COMPLETER >
    return tmr->CNT;
}

