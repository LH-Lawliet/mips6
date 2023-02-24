#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"


#define MAIN1


#ifdef MAIN1

#define IO_CFG_LED_RED      PIN_MODE_OUTPUT | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL | PIN_OPT_OUTPUT_SPEED_MEDIUM

#define BLINK_SPEED_BASE    1000

void tmr_cb()
{
    static uint32_t led_state = 0;
    led_state = !led_state;
	red_led(led_state);
}

int main()
{
    // Configure output LED_RED   --> PB4
	io_configure(_GPIOB,  PIN_4, IO_CFG_LED_RED, NULL);
    red_led(0);

	timer_tick_init(_TIM2, BLINK_SPEED_BASE, tmr_cb);
	timer_start(_TIM2);
	
	while(1){;}
    
	return 0;
}

#endif /* MAIN1 */



#ifdef MAIN2

#define SAMPLING_PERIOD     100     //us                             
#define BLINK_SPEED_BASE    1000    //ms

#define IO_CFG_LED_RED      //  < A COMPLETER >
                             
#define IO_CFG_SW_EXTI      //  < A COMPLETER >                            

void tmr_cb()
{
    static uint32_t led_state = 0;
	led_state = !led_state;
	red_led(led_state);
}

void exti5_cb()
{
    static uint32_t timer_on = 0;
        
    //  < A COMPLETER >

}

int main()
{
    lcd_reset(); cls();
    // Configure output LED_RED   --> PB4
//	io_configure(  < A COMPLETER >,  < A COMPLETER >, IO_CFG_LED_RED, NULL);
    red_led(0);

    // Configure input SW_CENTER --> PB5, with callback
	//  < A COMPLETER >

	
    //config timer _TIM2 
    timer_tick_init(_TIM2, BLINK_SPEED_BASE, tmr_cb);
    lcd_printf("program started");    
	while(1){;}    
}

#endif /* MAIN2 */



#ifdef MAIN3

#define SAMPLING_PERIOD     100     //us                            
#define BLINK_SPEED_BASE    1000    //ms

#define IO_CFG_SW_EXTI      //  < A COMPLETER >

#define IO_CFG_SW           //  < A COMPLETER >                             
                            
volatile uint32_t blink_speed = BLINK_SPEED_BASE;
volatile uint32_t color = LED_RED;

void tmr_cb()
{
    //  < A COMPLETER >
}

//sw center (PB5) callback
void exti5_cb()
{
    //  < A COMPLETER >
}

//sw up (PA4) callback
void exti4_cb()
{
    //  < A COMPLETER >
}

//sw down (PB0) callback
void exti0_cb()
{
    //  < A COMPLETER >
}

int main()
{
    lcd_reset(); cls();
    
    //leds config
    leds_init();
    leds(0);
    
    // Configure input SW_CENTER --> PB5, with callback
	//  < A COMPLETER >
    // Configure input SW_UP --> PA4, with callback
	//  < A COMPLETER >
    // Configure input SW_DOWN --> PB0, with callback
	//  < A COMPLETER >
    // Configure input SW_LEFT --> PC1, without callback
	//  < A COMPLETER >
	// Configure input SW_RIGHT --> PC0, without callback
	//  < A COMPLETER >
    
    //config timer _TIM2 
    timer_tick_init(_TIM2, BLINK_SPEED_BASE, tmr_cb);
    lcd_printf("program started");  
        
	while(1)
    {
        timer_wait_us(_TIM3, SAMPLING_PERIOD, NULL);
        
        //  < A COMPLETER >
    }    
}

#endif /* MAIN3 */


