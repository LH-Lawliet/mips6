#include "timer.h"

#include "libshield/lcd_128x32.h"

/* timer_wait_ms
 *   wait for ms millisecond function
 */
int timer_wait_ms(TIM_t *tmr, uint32_t ms, OnTick cb)
{
//	< A COMPLETER >
    timer_wait_us(tmr, ms*1000, cb);
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
    lcd_printf("start loop\n");
    while (tmr->CR1 & 1)
    {
        lcd_printf("loop\n");
    }
    return 0;

    /*
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
    */
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

