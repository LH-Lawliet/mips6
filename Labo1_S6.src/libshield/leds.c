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
 
#define IO_CFG_LEDS (    < A COMPLETER >    )


uint32_t leds_init(void)
{
    //          < A COMPLETER >
}

void leds(uint16_t val)
{
    //          < A COMPLETER >
}

void red_led(uint32_t on)
{
    //          < A COMPLETER >
}

void green_led(uint32_t on)
{
    //          < A COMPLETER >
}

void blue_led(uint32_t on)
{
    //          < A COMPLETER >
}
