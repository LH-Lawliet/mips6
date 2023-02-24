#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif 

#include "include/board.h"

typedef void (*OnTick)(void);

/***************************************************************************/
/* timer_wait_ms
 *   wait for ms milliseconds function
 */
int timer_wait_ms(TIM_t *tmr, uint32_t ms, OnTick cb);

/* timer_wait_us
 *   wait for us microseconds function
 */
int timer_wait_us(TIM_t *tmr, uint32_t us, OnTick cb);

/* timer_count_init
 *   setup timer to count each timebase_us us
 */
int timer_count_init(TIM_t *tmr, uint32_t timebase_us);

/* timer_start
 *   start counting to generate ticks
 */
void timer_start(TIM_t *tmr);

/* timer_stop
 *   stop and reset counting
 */
void timer_stop(TIM_t *tmr);

/* read_timer
 *   return the counter value
 */
uint32_t read_timer(TIM_t *tmr);


#ifdef __cplusplus
}
#endif
#endif
