#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"


#define MAIN1


#ifdef MAIN1

#define wait_us           1000*1000    //us

int main()
{    
    leds_init();
    leds(0);
    lcd_reset();   

    //cls();
    //lcd_printf("start loop\n");

    //timer_wait_us(_TIM2, 1000*1000, NULL);

    while(1)
    {
        red_led(1);
        timer_wait_us(_TIM2, wait_us, NULL);
        red_led(0);
        timer_wait_us(_TIM2, wait_us, NULL);
    }        
}            
            
#endif /* MAIN1 */



#ifdef MAIN2

#define sampling_period 100         //us
#define timebase_us     100

int main()
{
    uint32_t delay = 0;
    
    sw_init();
    timer_count_init(_TIM2, timebase_us);
    lcd_reset();    
    
    while(1)
    {
        //TIM3 used for sampling button states
        timer_wait_us(_TIM3, sampling_period, NULL);
        if(sw_center())
        {
            //TIM2 used for delay measurement
            timer_start(_TIM2);
            while(sw_input() & SW_CENTER);
            timer_stop(_TIM2);
            delay = read_timer(_TIM2);
            cls();
            lcd_printf("duree appui : %d ms\r\n", delay*timebase_us/1000);
        }
    }        
}
#endif /* MAIN2 */



#ifdef MAIN3

#define sampling_period 100         //us
#define timebase_us     100
#define long_delay      1000        //ms

int main()
{
    uint32_t delay = 0;
    uint8_t color = 0;
    uint8_t leds_state = 1;
    
    leds_init();
    leds(0);
    sw_init();
    lcd_reset();
    
    //initialisation timer
    
    
    while(1)
    {
        timer_wait_us(_TIM3, sampling_period, NULL);
        
//      < CODE PRINCIPAL A COMPLETER >
    }        
}
#endif /* MAIN3 */
                 
                            
