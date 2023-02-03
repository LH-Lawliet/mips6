#include "lib/io.h"
#include "sw.h"


/* switches
 *   SW_RIGHT  --> PC0
 *   SW_LEFT   --> PC1
 *   SW_UP     --> PA4
 *   SW_DOWN   --> PB0
 *   SW_CENTER --> PB5
 */
 
#define SW_RIGHT_MASK   (1<<0)
#define SW_RIGHT_OFST   (0)
#define SW_LEFT_MASK    (1<<1)
#define SW_LEFT_OFST    (1)
#define SW_UP_MASK      (1<<4)
#define SW_UP_OFST      (4)
#define SW_DOWN_MASK    (1<<0)
#define SW_DOWN_OFST    (0)
#define SW_CENTER_MASK  (1<<5)
#define SW_CENTER_OFST  (5)


uint32_t sw_init(void)
{
	//          < A COMPLETER >    
}

uint32_t sw_right()
{
	static uint16_t btn_state = 0;
	btn_state = (btn_state<<1) | ((((io_read(_GPIOC, SW_RIGHT_MASK)) >> SW_RIGHT_OFST) & 1) & 0x3FFF);
	if (btn_state == 0x1FFF) return 1;
	return 0;
}

uint32_t sw_left()
{
	static uint16_t btn_state = 0;
	btn_state = (btn_state<<1) | ((((io_read(_GPIOC, SW_LEFT_MASK)) >> SW_LEFT_OFST) & 1) & 0x3FFF);
	if (btn_state == 0x1FFF) return 1;
	return 0;
}

uint32_t sw_up()
{
	static uint16_t btn_state = 0;
	btn_state = (btn_state<<1) | ((((io_read(_GPIOA, SW_UP_MASK)) >> SW_UP_OFST) & 1) & 0x3FFF);
	if (btn_state == 0x1FFF) return 1;
	return 0;
}

uint32_t sw_down()
{
	static uint16_t btn_state = 0;
	btn_state = (btn_state<<1) | ((((io_read(_GPIOB, SW_DOWN_MASK)) >> SW_DOWN_OFST) & 1) & 0x3FFF);
	if (btn_state == 0x1FFF) return 1;
	return 0;
}

uint32_t sw_center()
{
	static uint16_t btn_state = 0;
	btn_state = (btn_state<<1) | ((((io_read(_GPIOB, SW_CENTER_MASK)) >> SW_CENTER_OFST) & 1) & 0x3FFF);
	if (btn_state == 0x1FFF) return 1;
	return 0;
}

uint32_t sw_input() {
    //          < A COMPLETER >
}

