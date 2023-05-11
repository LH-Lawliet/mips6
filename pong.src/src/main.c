#include "include/board.h"
#include "lib/io.h"
#include "lib/adc.h"
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
float ball_xVel = 35; //pixel/s
float ball_yVel = 15; //pixel/S
float ball_ratio = 0.2;
float bounce = 0;

uint8_t game_yPlayer1 = 5;
uint8_t game_yPlayer2 = 5;
uint8_t game_border = 15;
uint8_t game_barHeight = 32/4;

uint8_t game_scorePlayer1 = 0;
uint8_t game_scorePlayer2 = 0;


uint8_t isCatched() {
    uint8_t catchingMargin = 1;
    if ((ball_y+catchingMargin)>game_yPlayer1 && ball_y<(game_yPlayer1+game_barHeight+catchingMargin)) {
        ball_yVel = 15* (ball_y-(game_yPlayer1))/game_barHeight;
        return 1;
    } else if ((ball_y+catchingMargin)>game_yPlayer2 && ball_y<(game_yPlayer2+game_barHeight+catchingMargin)) {
        ball_yVel = 15* (ball_y-(game_yPlayer2))/game_barHeight;
        return 2;
    }
    return 0;
}

void resetBall() {
    ball_x = 128/2;
    ball_y = 32/2;
    ball_yVel = 15;
    bounce = 0;
    timer_wait_us(_TIM2, 1000000, NULL); //one sec
}

void drawMiddleLine() {
    uint8_t lineCount = 10;
    for (uint8_t i = 0; i<lineCount; ++i) {
        if (i%2) {
            line(128/2, 32/lineCount*i, 128/2, 32/lineCount*(i+1), 1);
        }
    }
}

void moveBall(uint32_t step) { //step is in us

    ball_x += (ball_xVel*(1+(ball_ratio*bounce)))/1000000*step;
    ball_y += (ball_yVel*(1+(ball_ratio*bounce)))/1000000*step;

    if (ball_y>(32-ball_r) || ball_y<(0+ball_r)) {
        ball_yVel = -ball_yVel;
    };


    uint8_t limitLeft = ball_x<(game_border+ball_r);
    uint8_t limitRight = ball_x>(132-game_border-2*ball_r);

    if (limitLeft || limitRight) {
        ball_xVel = -ball_xVel;
        ball_ratio = ball_ratio*ball_ratio;

        if (isCatched()==0) {
            leds(4);
            if (limitLeft) {
                game_scorePlayer2 += 1;
            } else {
                game_scorePlayer1 += 1;
            }
            resetBall();
        } else {
            leds(2);
            bounce+=1;
        }
    }
}

void printScores() {
    locate(0,0);
    lcd_printf("%d", game_scorePlayer1);
    locate(128-5,0);
    lcd_printf("%d", game_scorePlayer2);
}

int main()
{    
    leds_init();
    leds(0);
    lcd_reset();   


    //io_configure(_GPIOA, PIN_0, ( PIN_MODE_ANALOG | PIN_OPT_NONE ), NULL);
    //io_configure(_GPIOA, PIN_1, ( PIN_MODE_ANALOG | PIN_OPT_NONE ), NULL);

    int f = adc_init(_ADC1, ADC_MODE_12BITS|ADC_MODE_SINGLE, NULL);

    //ADC1->DR; //this is the output
    cls();
    lcd_printf("%d\n", ADC1->DR);

    
    while(1)
    {
        timer_wait_us(_TIM2, wait_us, NULL);
        cls();
        leds(0);

        drawMiddleLine();

        game_yPlayer1 = (uint8_t) (((float) adc_channel_sample(_ADC1, 0)/4096)*(32-game_barHeight));
        game_yPlayer2 = (uint8_t) (((float) adc_channel_sample(_ADC1, 1)/4096)*(32-game_barHeight));

        line(game_border, game_yPlayer1, game_border, game_yPlayer1+game_barHeight, 1);
        line(128-game_border, game_yPlayer2, 128-game_border, game_yPlayer2+game_barHeight, 1);

        circle((uint32_t) ball_x, (uint32_t) ball_y, ball_r, 1);

        printScores();

        moveBall(wait_us);
    }      
}            