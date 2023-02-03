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


#define IO_CFG_SW ( PIN_MODE_INPUT | PIN_OPT_RESISTOR_NONE )


uint32_t sw_init(void)
{
	//          < A COMPLETER >    
	uint32_t result_swr = io_configure(_GPIOC, SW_LEFT_MASK, IO_CFG_SW, NULL);
	uint32_t result_swl = io_configure(_GPIOC, SW_RIGHT_MASK, IO_CFG_SW, NULL);
    uint32_t result_swu = io_configure(_GPIOA, SW_UP_MASK, IO_CFG_SW, NULL);
    uint32_t result_swd = io_configure(_GPIOB, SW_DOWN_MASK, IO_CFG_SW, NULL);
    uint32_t result_swc = io_configure(_GPIOB, SW_CENTER_MASK, IO_CFG_SW, NULL);

    return result_swr | result_swl | result_swu | result_swd | result_swc;
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
	uint32_t returnVal = io_read(_GPIOC, SW_RIGHT_MASK)>>SW_RIGHT_OFST;
	returnVal |= io_read(_GPIOC, SW_LEFT_MASK)>>SW_LEFT_OFST<<1;
	returnVal |= io_read(_GPIOA, SW_UP_MASK)>>SW_UP_OFST<<2;
	returnVal |= io_read(_GPIOB, SW_DOWN_MASK)>>SW_DOWN_OFST<<3;
	returnVal |= io_read(_GPIOB, SW_CENTER_MASK)>>SW_CENTER_OFST<<4;


	/*
	uint32_t result_swr = io_configure(_GPIOC, SW_LEFT_MASK|SW_RIGHT_MASK, IO_CFG_SW, NULL);
    uint32_t result_swu = io_configure(_GPIOA, SW_UP_MASK, IO_CFG_SW, NULL);
    uint32_t result_swd = io_configure(_GPIOB, SW_DOWN_MASK | SW_CENTER_MASK, IO_CFG_SW, NULL);


    uint32_t returnVal = sw_right();
    returnVal |= (sw_left()<<1);
    returnVal |= (sw_up()<<2);
    returnVal |= (sw_down()<<3);
    returnVal |= (sw_center()<<4);*/
    return returnVal;
}

