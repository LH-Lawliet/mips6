#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/adc.h"
#include "libshield/sw.h"
#include "libshield/leds.h"
#include "libshield/lcd_128x32.h"

#define MAIN3

/******************************************************************************
 * util functions
 *
 ******************************************************************************/
// clear the screen
void clear(USART_t *u)
{
	uart_puts(u, "\x1B[2J\x1B[H\x1b[?25l");
}

// goto position row, col and erase the line from the current poistion to the
// end of line
void gotoxy(USART_t *u, uint32_t r, uint32_t c)
{
	uart_printf(u, "\x1B[%d;%dH", r, c);
}

/******************************************************************************
 * FIFO buffer handling
 ******************************************************************************/
#define BUFFER_SIZE				50

#define FIFO_OK     0
#define FIFO_FULL   -1
#define FIFO_EMPTY  -1

typedef struct _Fifo {
	volatile uint16_t	channel[BUFFER_SIZE];
	volatile uint16_t	data[BUFFER_SIZE];
	volatile uint32_t	i_wr;
	uint32_t			i_rd;
	uint32_t			cnt;
} Fifo;

static void buf_init(Fifo *f)
{
	f->cnt=0;
	f->i_wr = f->i_rd = 0;
}

static int buf_write(Fifo *f, uint16_t chan, uint16_t val)
{
	if (f->cnt ==BUFFER_SIZE) return FIFO_FULL;
	f->channel[f->i_wr] = chan;
	f->data[f->i_wr] = val;
	f->i_wr = (f->i_wr + 1) % BUFFER_SIZE;
	f->cnt++;
	return FIFO_OK;
}

static int buf_read(Fifo *f, uint16_t *chan, uint16_t *val)
{
	if (f->cnt == 0) return FIFO_EMPTY;
	*chan = f->channel[f->i_rd];
	*val = f->data[f->i_rd];
	f->i_rd = (f->i_rd + 1) % BUFFER_SIZE;
	f->cnt--;
	return FIFO_OK;
}

#ifdef MAIN1
/******************************************************************************
 * adc: polling
 ******************************************************************************/

volatile char cmd=0;
uint16_t val;

static void on_rx_cb(char c)
{
	cmd = c;
}

int main()
{
	sw_init();
	lcd_reset();
	int f = adc_init(_ADC1, ADC_MODE_12BITS|ADC_MODE_SINGLE, NULL);
	
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	clear(_USART2);
	
	while (1) {
		uart_printf(_USART2,"\r\nEntrez une commande : ");
		while (!cmd) ;
		switch (cmd) {
		case '0':
			val = adc_channel_sample(_ADC1, 0);
			uart_printf(_USART2, "\r\nchannel %d : val = %d", 0, val);
			break;
		case '1':
			val = adc_channel_sample(_ADC1, 1);
			uart_printf(_USART2, "\r\nchannel %d : val = %d", 1, val);
			break;
		case 'b':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_VBAT);
			uart_printf(_USART2, "\r\nVBAT       : val = %d", val);
			break;
		case 'r':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_VREFINT);
			uart_printf(_USART2, "\r\nVREFINT    : val = %d", val);
			break;
		case 't':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_TEMP);
			uart_printf(_USART2, "\r\ntemp       : val = %d [%d]", (int)(((val*3.265/4096.0)-0.76)/0.0025+25.0), val); /* A MODIFIER */
			break;
		}
		cmd=0;
	}

	return 1;
}

#endif

#ifdef MAIN2
/******************************************************************************
 * adc: irq
 ******************************************************************************/
Fifo buf;

volatile char cmd=0;

static void on_eoc(uint16_t channel, uint16_t val)
{
	buf_write(&buf, channel, val);
}

static void on_rx_cb(char c)
{
	cmd = c;
}

uint16_t channel=1, val;

int main()
{
	sw_init();
	lcd_reset();
	
	buf_init(&buf);
	int f = adc_init(_ADC1, ADC_MODE_12BITS|ADC_MODE_SINGLE, on_eoc);
		
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	clear(_USART2);
	
	while (1) {
		uart_printf(_USART2,"\r\nEntrez une commande : ");
		while (!cmd) ;
		switch (cmd) {
		case '0':
			val = adc_channel_sample(_ADC1, 0);
			break;
		case '1':
			val = adc_channel_sample(_ADC1, 1);
			break;
		case 'b':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_VBAT);
			break;
		case 'r':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_VREFINT);
			break;
		case 't':
			val = adc_channel_sample(_ADC1, ADC_CHANNEL_TEMP);
			break;
		}
		cmd=0;
		
		if (buf_read(&buf, &channel, &val) == 0) {
			switch (channel) {
			case 0:
			case 1:
				uart_printf(_USART2, "\r\nchannel %d : val = %d", channel, val);
				break;
			case ADC_CHANNEL_VBAT:
				uart_printf(_USART2, "\r\nVBAT       : val = %d", val);
				break;
			case ADC_CHANNEL_VREFINT:
				uart_printf(_USART2, "\r\nVREFINT    : val = %d", val);
				break;
			case ADC_CHANNEL_TEMP:
				uart_printf(_USART2, "\r\ntemp       : val = %d", (int)(((val*3.3/4096.0)-0.76)/0.0025+25.0));
				break;
			}
		}
	}

	return 1;
}

#endif

#ifdef MAIN3
Fifo buf;
volatile int overrun;
int underrun=0;

static void on_eoc(uint16_t channel, uint16_t val)
{
	if (buf_write(&buf, channel, val) == 0) {
		overrun = 0;
	} else {
		overrun = 1;
	}
}

uint16_t channel, val;

int main()
{
	sw_init();
	lcd_reset();
	
	buf_init(&buf);
	int f = adc_init(_ADC1, ADC_MODE_12BITS|ADC_MODE_CONTINUOUS, on_eoc);
	adc_channel_enable(_ADC1, 0, ADC_SMPL_480CYCLES);
	adc_channel_enable(_ADC1, 1, ADC_SMPL_480CYCLES);
	adc_channel_enable(_ADC1, ADC_CHANNEL_TEMP, ADC_SMPL_480CYCLES);
	
	int fs = adc_get_sampling_freq(_ADC1);
	
	uart_init(_USART2, 115200, UART_8N1, NULL);
	clear(_USART2);
	gotoxy(_USART2, 7, 1);uart_printf(_USART2, "ADC freq       = %d Hz", f);
	gotoxy(_USART2, 8, 1);uart_printf(_USART2, "ADC sampl freq = %d Hz", fs);
	
	while (1) {
		
		if (sw_center()) {
			adc_start(_ADC1);
		}
		
		if (buf_read(&buf, &channel, &val) == 0) {
			underrun = 0;
			if (channel==0 || channel==1) {
				gotoxy(_USART2, channel+1, 1);
			} else {
				gotoxy(_USART2, 3, 1);
			}
			uart_printf(_USART2, "channel %d : val = %d   ", channel, val);
		} else {
			underrun = 1;
		}
		
		gotoxy(_USART2, 6, 1);
		if (overrun) {
			uart_puts(_USART2, "overrun ");
		} else if (underrun) {
			uart_puts(_USART2, "underrun");
		}
	}

	return 1;
}
#endif

#ifdef MAIN4
Fifo buf;
volatile int overrun;
int underrun=0;

uint32_t state=0;

void tmr_cb()
{
	state=!state;
	red_led(state);
}

static void on_eoc(uint16_t channel, uint16_t val)
{
	if (buf_write(&buf, channel, val) == 0) {
		overrun = 0;
	} else {
		overrun = 1;
	}
}

uint16_t channel, val;

int main()
{
	sw_init();
	leds_init();
	lcd_reset();
	
	// Set sampling period
	timer_tick_init(_TIM2, 250, tmr_cb);
//	timer_tick_init(_TIM2, 250, NULL);
	
	buf_init(&buf);
	int f = adc_init(_ADC1, ADC_MODE_12BITS|ADC_MODE_TRIGGERED, on_eoc);
	adc_channel_enable(_ADC1, 0, ADC_SMPL_480CYCLES);
	adc_channel_enable(_ADC1, 1, ADC_SMPL_480CYCLES);
	adc_channel_enable(_ADC1, ADC_CHANNEL_TEMP, ADC_SMPL_480CYCLES);
	adc_set_trigger_evt(_ADC1, ADC_TRIG_ON_TIM2_TRGO);
	
	int fs = adc_get_sampling_freq(_ADC1);
	
	uart_init(_USART2, 115200, UART_8N1, NULL);
	clear(_USART2);
	gotoxy(_USART2, 7, 1);uart_printf(_USART2, "ADC freq       = %d Hz", f);
	gotoxy(_USART2, 8, 1);uart_printf(_USART2, "ADC sampl freq = %d Hz", fs);
	
	leds(0);

	while (1) {
		
		if (sw_center()) {
			timer_start(_TIM2);
		}
		
		if (buf_read(&buf, &channel, &val) == 0) {
			underrun = 0;
			if (channel==0 || channel==1) {
				gotoxy(_USART2, channel+1, 1);
			} else {
				gotoxy(_USART2, 3, 1);
			}
			uart_printf(_USART2, "channel %d : val = %d   ", channel, val);
		} else {
			underrun = 1;
		}
		
		gotoxy(_USART2, 6, 1);
		if (overrun) {
			uart_puts(_USART2, "overrun ");
		} else if (underrun) {
			uart_puts(_USART2, "underrun");
		}
	}

	return 1;
}

#endif
