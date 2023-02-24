#include "include/board.h"
#include "lib/io.h"
#include "lib/timer.h"
#include "libshield/lcd_128x32.h"
#include "libshield/leds.h"
#include "libshield/sw.h"


#define wait_us           33*1000    //us


#define POT1                        // pa0
#define POT2                        // pa1

float ball_x = 128/2;
float ball_y = 32/2;
uint8_t ball_r = 2;
float ball_xVel = 25; //pixel/s
float ball_yVel = 10; //pixel/S

uint8_t game_yPlayer1 = 5;
uint8_t game_yPlayer2 = 5;
uint8_t game_border = 30;
uint8_t game_barHeight = 32/4;


void moveBall(uint32_t step) { //step is in us
    ball_x += ball_xVel/1000000*step;
    ball_y += ball_yVel/1000000*step;

    if (ball_y>(32-ball_r) || ball_y<(0+ball_r)) {
        ball_yVel = -ball_yVel;
    };


    uint8_t limitLeft = ball_x<(game_border+ball_r);
    uint8_t limitRight = ball_x>(132-game_border-2*ball_r);

    if (limitLeft || limitRight) {
        ball_xVel = -ball_xVel;
    }
}


int main()
{    
    leds_init();
    leds(0);
    lcd_reset();   


    io_configure(_GPIOA, PIN_0, ( PIN_MODE_ANALOG | PIN_OPT_NONE ), NULL);
    io_configure(_GPIOA, PIN_1, ( PIN_MODE_ANALOG | PIN_OPT_NONE ), NULL);


    ADC1->CR2 |= ADC_CR2_ADON|ADC_CR2_DMA; //start the adc
    ADC1->SR = ADC_CR1_RES;
    ADC1->CR1 |= ADC_CR1_RES | ADC_CR1_SCAN;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    
    ADC1->SMPR1 |= ADC_SMPR2_SMP0;

    ADC1->SQR1 &= 0xFF0FFFFF;
    ADC1->SQR3 &= 0xFFFFFF00;


    ADC1->CR2 |= ADC_CR2_SWSTART; //start the conversion


    while (!(ADC1->CR2&ADC_SR_EOC)) {
        cls();
        lcd_printf("%d %d\n", ADC1->CR1,ADC_SR_EOC);
    }

    //ADC1->DR; //this is the output
    cls();
    lcd_printf("%d\n", ADC1->DR);




    /*
    while(1)
    {
        timer_wait_us(_TIM2, wait_us, NULL);
        cls();

        game_yPlayer1 = io_read(_GPIOA, PIN_0);
        game_yPlayer2 = io_read(_GPIOA, PIN_1);

        lcd_printf("%d %d\n", game_yPlayer1,game_yPlayer2);


        line(game_border, game_yPlayer1, game_border, game_yPlayer1+game_barHeight, 1);
        line(128-game_border, game_yPlayer2, 128-game_border, game_yPlayer2+game_barHeight, 1);

        circle((uint32_t) ball_x, (uint32_t) ball_y, ball_r, 1);

        moveBall(wait_us);
    }      */ 
}            