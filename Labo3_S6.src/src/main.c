#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"


#define MAIN3


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

#define IO_CFG_LED_RED      PIN_MODE_OUTPUT | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL | PIN_OPT_OUTPUT_SPEED_MEDIUM
                             
#define IO_CFG_SW_EXTI      PIN_MODE_INPUT | PIN_OPT_RESISTOR_NONE | PIN_OPT_IRQ_EDGE_RISE                           

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

    if (timer_on) {
        timer_stop(_TIM2);
    } else {
        timer_start(_TIM2);
    }
    timer_on = !timer_on;

    cls();
    lcd_printf("Timer is now %d", timer_on);  
}

int main()
{
    lcd_reset(); cls();
    // Configure output LED_RED   --> PB4
//	io_configure(  < A COMPLETER >,  < A COMPLETER >, IO_CFG_LED_RED, NULL);
    io_configure(_GPIOB,  PIN_4, IO_CFG_LED_RED, NULL);
    red_led(0);

    // Configure input SW_CENTER --> PB5, with callback
    io_configure(_GPIOB,  PIN_5, IO_CFG_SW_EXTI, exti5_cb);
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

#define IO_CFG_SW           PIN_MODE_INPUT | PIN_OPT_RESISTOR_NONE                             
#define IO_CFG_SW_EXTI      PIN_MODE_INPUT | PIN_OPT_RESISTOR_NONE | PIN_OPT_IRQ_EDGE_RISE                        
                            
volatile uint32_t blink_speed = BLINK_SPEED_BASE;
volatile uint32_t color = LED_RED;

void tmr_cb()
{
    static uint32_t led_state = 0;
    led_state = !led_state;
    leds(led_state*color);
}

//sw center (PB5) callback
void exti5_cb()
{
    static uint32_t timer_on = 0;
    if (timer_on) {
        timer_stop(_TIM2);
    } else {
        timer_start(_TIM2);
    }
    timer_on = !timer_on;

    cls();
    lcd_printf("Timer is now %d", timer_on); 
}

//sw up (PA4) callback
void exti4_cb()
{
    if (blink_speed>10) {
        blink_speed = blink_speed/2;
        timer_tick_period(_TIM2, blink_speed);
    }
    cls();
    lcd_printf("Timer is now %dms", blink_speed); 
}

//sw down (PB0) callback
void exti0_cb()
{
    if (blink_speed<5000) {
        blink_speed = blink_speed*2;
        timer_tick_period(_TIM2, blink_speed);
    }
    cls();
    lcd_printf("Timer is now %dms", blink_speed); 
}

int main()
{
    lcd_reset(); cls();
    
    //leds config
    leds_init();
    leds(0);
    
    // Configure input SW_CENTER --> PB5, with callback
	//  < A COMPLETER >
    io_configure(_GPIOB,  PIN_5, IO_CFG_SW_EXTI, exti5_cb);
    // Configure input SW_UP --> PA4, with callback
	//  < A COMPLETER >
    io_configure(_GPIOA,  PIN_4, IO_CFG_SW_EXTI, exti4_cb);
    // Configure input SW_DOWN --> PB0, with callback
	//  < A COMPLETER >
    io_configure(_GPIOB,  PIN_0, IO_CFG_SW_EXTI, exti0_cb);
    // Configure input SW_LEFT --> PC1, without callback
	//  < A COMPLETER >
    io_configure(_GPIOC,  PIN_1, IO_CFG_SW, NULL);
	// Configure input SW_RIGHT --> PC0, without callback
	//  < A COMPLETER >
    io_configure(_GPIOC,  PIN_0, IO_CFG_SW, NULL);

    
    //config timer _TIM2 
    timer_tick_init(_TIM2, BLINK_SPEED_BASE, tmr_cb);
    lcd_printf("program started");  
        
	while(1)
    {
        timer_wait_us(_TIM3, SAMPLING_PERIOD, NULL);
        //  < A COMPLETER > 
        
        if (sw_right()) {
            if (color==LED_RED) {color=LED_GREEN;}
            else if (color==LED_GREEN) {color=LED_BLUE;}
            else if (color==LED_BLUE) {color=LED_RED;};
        } else if (sw_left()) {
            if (color==LED_RED) {color=LED_BLUE;}
            else if (color==LED_GREEN) {color=LED_RED;}
            else if (color==LED_BLUE) {color=LED_GREEN;};
        }

    }    
}

#endif /* MAIN3 */


