#include "lib/io.h"
#include "leds.h"

/* led RGB :
 *   LED_RED   --> PB4
 *   LED_GREEN --> PC7
 *   LED_BLUE  --> PA9
 */
#define LED_RED_MASK	(1 << 4)
#define LED_GREEN_MASK	(1 << 7)
#define LED_BLUE_MASK	(1 << 9)
#define LED_RED_OFST    4
#define LED_GREEN_OFST  7
#define LED_BLUE_OFST   9
 
#define IO_CFG_LEDS ( PIN_MODE_OUTPUT | PIN_OPT_RESISTOR_NONE | PIN_OPT_OUTPUT_PUSHPULL | PIN_OPT_OUTPUT_SPEED_MEDIUM )


uint32_t leds_init(void)
{
    //          < A COMPLETER >
    uint32_t result_r = io_configure(_GPIOB, PIN_4, IO_CFG_LEDS, NULL);
    uint32_t result_g = io_configure(_GPIOC, PIN_7, IO_CFG_LEDS, NULL);
    uint32_t result_b = io_configure(_GPIOA, PIN_9, IO_CFG_LEDS, NULL);
    
    return result_r | result_g | result_b;
}

void leds(uint16_t val)
{
    //          < A COMPLETER >
    //io_set(_GPIOC, PIN_MODE_OUTPUT);

    red_led(val&LED_RED);
    green_led(val&LED_GREEN);
    blue_led(val&LED_BLUE);

}

void red_led(uint32_t on)
{
    //          < A COMPLETER >
    if (on) {
        io_write_n(_GPIOB, LED_RED_MASK, LED_RED_MASK);
    } else {
        io_write_n(_GPIOB, 0, LED_RED_MASK);
    }
}

void green_led(uint32_t on)
{
    //          < A COMPLETER >
    if (on) {
        io_write_n(_GPIOC, LED_GREEN_MASK, LED_GREEN_MASK);
    } else {
        io_write_n(_GPIOC, 0, LED_GREEN_MASK);
    }
}

void blue_led(uint32_t on)
{
    //          < A COMPLETER >
    if (on) {
        io_write_n(_GPIOA, LED_BLUE_MASK, LED_BLUE_MASK);
    } else {
        io_write_n(_GPIOA, 0, LED_BLUE_MASK);
    }
}
