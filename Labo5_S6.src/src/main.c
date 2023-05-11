#include <stdio.h>
#include "include/board.h"
#include "lib/uart.h"
#include "lib/util.h"


#define MAIN4


#ifdef MAIN1
/***************************************************************************
 * uart : test émission (polling) et réception (polling) de caractères
 *
 ***************************************************************************/
int main()
{
	uart_init(_USART2,115200,UART_8N1,NULL);

	uart_putc(_USART2, 'A');
	uart_putc(_USART2, 'B');
	uart_putc(_USART2, 'C');
	
	uart_puts(_USART2,"\r\nC'est un message du STM32F411 :-)\r\n");

	uart_pu
	uart_printf(_USART2, "\x1B[%u;%uHle pointeur _USART2 = 0x%x\n", 20,5,_USART2);
s(_USART2, "Entrez du texte ici : ");
	uart_printf(_USART2, "\x1B[%u;%uHle pointeur _USART2 = 0x%x\n", 20,5,_USART2);

	while (1) {
		uart_putc(_USART2, uart_getc(_USART2));
	}

	return 1;
}
#endif


#ifdef MAIN2
/***************************************************************************
 * uart : test émission (polling) et réception (irq) de caractères
 *
 ***************************************************************************/
static void on_rx_cb(char c)
{
	uart_putc(_USART2, c);
}


int main()
{
	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);

	uart_puts(_USART2, "Entrez du texte ici : ");

	while (1) ;

	return 1;
}
#endif


#ifdef MAIN3
/***************************************************************************
 * uart : codes hexa reçus
 *
 ***************************************************************************/
static void on_rx_cb(char c)
{
	char  s[34];
	num2str(s,c,16);
	uart_puts(_USART2, " 0x");
	uart_puts(_USART2, s);
}


int main()
{
	uart_init(_USART2,115200,UART_8N1,on_rx_cb);

	uart_puts(_USART2,"\x1B[2J\x1B[H");

	uart_puts(_USART2,"On affiche un message ici");

	// positionnement du curseur ligne 20, col 5
	uart_puts(_USART2,"\x1B[20;5H");

	// on écrit en couleur
	uart_puts(_USART2,"\x1B[31mA partir de maintenant, entrez du texte :\x1B[0m");

	while (1) ;

	return 1;
}
#endif


#ifdef MAIN4
/***************************************************************************
 * printf : test
 *
 ***************************************************************************/
static void on_rx_cb(char c)
{
	char  s[34];
	num2str(s,c,16);
	uart_puts(_USART2, " 0x");
	uart_puts(_USART2, s);
}

int main()
{
	int a = 5, b = 8;

	uart_init(_USART2, 115200, UART_8N1, on_rx_cb);

	uart_puts(_USART2, "\x1B[2J\x1B[H");

	uart_printf(_USART2, "\r %s \n", "Une belle string");


	uart_printf(_USART2, "la somme de %d et %d est %d\n", a, b, a+b);
	uart_printf(_USART2, "\x1B[%u;%uHle pointeur _USART2 = 0x%x\n", 20,5,_USART2);
    
	while(1) {
	}

	return 0;
}
#endif
