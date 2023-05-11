#include "include/board.h"
#include "lib/io.h"
#include "lib/uart.h"
#include "lib/util.h"
#include "lib/i2c.h"
#include "libshield/lm75.h"
#include "libshield/mma7660.h"

#define MMA0

volatile char cmd;
char buf[10];

static void on_rx_cb(char c)
{
	cmd = c;
}

int32_t buffer[4], moy[4]={0,0,0,0}, tmp;
int k=0;

int main()
{
	int temp;

	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);
	i2c_master_init(_I2C1);
	
	mma7660_setup(MMA7660_AM16|MMA7660_ACTIVE);
	
	while(1) {
#ifdef MMA0
		uart_printf(_USART2,"\r\nEntrez une commande : ");
#endif
		while (!cmd) ;
		switch (cmd) {
		case 't': // get current temperature
			lm75_read_temp(&temp);
			cmd=0;
			  // A MODIFIER
			uart_printf(_USART2,"\n\rLa température est %d.%d°C",temp,temp);
			break;
		case 'm':
			mma7660_read_XYZT(buffer);
			cmd=0;
#ifdef MMA0
			uart_printf(_USART2,"\n\rX=%d, Y=%d, Z=%d, T=0x%x\n\r  position: ", buffer[0], buffer[1], buffer[2], buffer[3]);
			tmp=buffer[3] & 3;
			if (tmp==1) {
				uart_puts(_USART2,"front, ");
			} else if (tmp==2) {
				uart_puts(_USART2,"back, ");
			}
			tmp= (buffer[3]>>2) & 7;
			switch (tmp) {
			case 1:
				uart_puts(_USART2,"left, ");
				break;
			case 2:
				uart_puts(_USART2,"right, ");
				break;
			case 5:
				uart_puts(_USART2,"down, ");
				break;
			case 6:
				uart_puts(_USART2,"up, ");
				break;
			}
			if (buffer[3] & (1<<5)) uart_puts(_USART2,"tapped, ");

			if (buffer[3] & (1<<7)) uart_puts(_USART2,"shaken");
#endif
#ifdef MMA1
			for (int i=0;i<4;i++) {
				uart_putc(_USART2, (char)buffer[i]);
			}

#endif
#ifdef MMA2
			k++;
			for (int i=0;i<4;i++) {
				moy[i] += buffer[i];
			}
			if (k==4) {
				for (int i=0;i<4;i++) {
					moy[i] = moy[i]/4;
					uart_putc(_USART2, (char)moy[i]);
					moy[i]=0;
				}
				k=0;
			}
#endif
			break;
		default:
			cmd=0;
			break;
		}
	}
	return 0;
}


