#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"



#define MAIN1

#ifdef MAIN1


int main()
{
    leds_init();
    sw_init();
    lcd_reset();
    
    //TESTS
    //function leds
    cls();
    lcd_printf("test fonction leds");
    leds(0);
    leds(LED_RED);                          //red led only
    timer_wait_ms(_TIM2, 1000, NULL);
    leds(LED_GREEN);                        //green led only
    timer_wait_ms(_TIM2, 1000, NULL);
    leds(LED_BLUE);                         //blue led only
    timer_wait_ms(_TIM2, 1000, NULL);
    leds(LED_RED | LED_GREEN | LED_BLUE);   //white color
    timer_wait_ms(_TIM2, 1000, NULL);
    leds(0);                                //leds turned off
    timer_wait_ms(_TIM2, 1000, NULL);
    //function red_led
    cls();
    lcd_printf("test red_led");
    red_led(1);                             //red led only    
    timer_wait_ms(_TIM2, 1000, NULL);
    //function green_led
    red_led(0);
    cls();
    lcd_printf("test green_led");
    green_led(1);                           //green led only
    timer_wait_ms(_TIM2, 1000, NULL);
    //function blue_led
    green_led(0);
    cls();
    lcd_printf("test blue_led");
    blue_led(1);                            //blue led only
    timer_wait_ms(_TIM2, 1000, NULL);
    blue_led(0);
    timer_wait_ms(_TIM2, 1000, NULL);
    
    while(1)
    {
        cls();
        lcd_printf("joystick value: %d", sw_input());
        timer_wait_ms(_TIM2, 100, NULL);
    }
        
}
#endif /* MAIN1 */


#ifdef MAIN2

#define pwm_period 10               //ms
#define sampling_period 100         //us

int main()
{
	uint32_t dutycycle = 0;
	
    lcd_reset();
    // Configure switches
    sw_init();
    
    // Configure output LED_RED:
//	io_configure(    A COMPLETER    );
    
//	pwm_init(    A COMPLETER    );
    
//	pwm_channel_enable(    A COMPLETER    );
 	
//	pwm_start(    A COMPLETER    );
	
	while (1) {
//        timer_wait_us(    A COMPLETER    );
        

//          < CODE PRINCIPAL A COMPLETER >
        
	}
	
	return 0;
}
#endif /* MAIN2 */

#ifdef MAIN3

#define pwm_period 10               //ms
#define sampling_period 100         //us
#define sw_up_delay_base 5000       //delay = n * sampling_period
#define sw_down_delay_base 5000     //delay = n * sampling_period
    
/* main test program */
int main()
{
	uint32_t dutycycle = 0;
    uint32_t sw_up_delay = 0;
    uint32_t sw_down_delay = 0;
	
    lcd_reset();
    // Configure switches
    sw_init();
    
    // Configure output LED_RED:
//	io_configure(    A COMPLETER    );
	// Configure output LED_GREEN:
//	io_configure(    A COMPLETER    );
    
	
    //PWM configurations
//      A COMPLETER        
	
	while (1) {
//      timer_wait_us(    A COMPLETER    );
        
        //          < CODE PRINCIPAL A COMPLETER >

        
	}
	
	return 0;
}
#endif /* MAIN3 */                     
                            
