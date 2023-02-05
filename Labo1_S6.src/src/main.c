#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"



#define MAIN3

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
    io_configure(_GPIOB, PIN_4, ( PIN_MODE_ALTFUNC | PIN_OPT_AF2 | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL ), NULL);
    
    //	pwm_init(    A COMPLETER    );
    pwm_init(_TIM3, pwm_period, NULL);
    
    //pwm_channel_enable(    A COMPLETER    ); 
    pwm_channel_enable(_TIM3, PWM_CHANNEL_1, dutycycle, 1); //oe seems to be output enable

    cls();
    lcd_printf("dutycycle: %d", dutycycle);
 	
    //	pwm_start(    A COMPLETER    );
	pwm_start(_TIM3);

	while (1) {
        //          < CODE PRINCIPAL A COMPLETER >  
        uint32_t left = sw_left();
        uint32_t right = sw_right();
        if (left && dutycycle>0) {
            dutycycle -= 10;
        } else if (right && dutycycle<100) {
            dutycycle += 10;
        }

        if (right || left) {
            pwm_channel_set(_TIM3, PWM_CHANNEL_1, dutycycle);
            cls();
            lcd_printf("dutycycle: %d", dutycycle);
        }

        timer_wait_us(_TIM2, sampling_period, NULL);            
	}
	
	return 0;
}
#endif /* MAIN2 */

#ifdef MAIN3

#define pwm_period 10               //ms
#define sampling_period 100         //us
#define sw_up_delay_base 5000       //delay = n * sampling_period
#define sw_down_delay_base 5000     //delay = n * sampling_period


#define SW_UP_MASK      (1<<4)
#define SW_DOWN_MASK    (1<<0)
    
/* main test program */
int main()
{
	uint32_t dutycycle = 50;
    uint32_t sw_up_delay = 0;
    uint32_t sw_down_delay = 0;
	
    lcd_reset();
    // Configure switches
    sw_init();

    cls();
    lcd_printf("dutycycle: %d", dutycycle);
    
    // Configure output LED_RED:
    //	io_configure(    A COMPLETER    );
    io_configure(_GPIOB, PIN_4, ( PIN_MODE_ALTFUNC | PIN_OPT_AF2 | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL ), NULL);

	// Configure output LED_GREEN:
    io_configure(_GPIOC, PIN_7, ( PIN_MODE_ALTFUNC | PIN_OPT_AF2 | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL ), NULL);
    //	io_configure(    A COMPLETER    );
    
	
    //PWM configurations
    //      A COMPLETER            
    pwm_init(_TIM3, pwm_period, NULL);
    
    //pwm_channel_enable(    A COMPLETER    ); 
    pwm_channel_enable(_TIM3, PWM_CHANNEL_1, dutycycle, 1); //red timer
    pwm_channel_enable(_TIM3, PWM_CHANNEL_2, 100-dutycycle, 1); //green timer

    pwm_start(_TIM3);

    void changeDutyCycle(uint32_t delta) {
        dutycycle += delta;

        if (dutycycle<0) {
            dutycycle = 0;
        } else if (dutycycle>100) {
            dutycycle = 100;
        }

        pwm_channel_set(_TIM3, PWM_CHANNEL_1, dutycycle);
        pwm_channel_set(_TIM3, PWM_CHANNEL_2, 100-dutycycle);
        cls();
        lcd_printf("dutycycle: %d %d", dutycycle, 100-dutycycle);
    }

	
	while (1) {
        uint32_t left = sw_left();
        uint32_t right = sw_right();
        if (left && dutycycle>0) {
            changeDutyCycle(-10);
        } else if (right && dutycycle<100) {
            changeDutyCycle(10);
        }

        
        if (io_read(_GPIOA, SW_UP_MASK)) {
            ++sw_up_delay;
            if (sw_up_delay>=sw_up_delay_base) {
                changeDutyCycle(10);
                sw_up_delay = 0;
            }
        } else {
            sw_up_delay = 0;
        }

        if (io_read(_GPIOB, SW_DOWN_MASK)) {
            ++sw_down_delay;
            if (sw_down_delay>=sw_down_delay_base) {
                changeDutyCycle(-10);
                sw_down_delay = 0;
            }
        } else {
            sw_down_delay = 0;
        }

        timer_wait_us(_TIM2, sampling_period, NULL); 
	}
	
	return 0;
}
#endif /* MAIN3 */                     
                            
