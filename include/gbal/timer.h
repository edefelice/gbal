#ifndef GBAL_TIMER_H
#define GBAL_TIMER_H

#include <stdint.h>
#include "gbal/irq.h"

typedef enum GBAL_TIMER {
    GBAL_TIMER0 = 0,
    GBAL_TIMER1,
    GBAL_TIMER2,
    GBAL_TIMER3,
    GBAL_TIMER_COUNT
} GbalTimer;

typedef enum GBAL_TIMER_PRESCALER {
    GBAL_TIMER_DIV_1 = 0,
    GBAL_TIMER_DIV_64,
    GBAL_TIMER_DIV_256,
    GBAL_TIMER_DIV_1024
} GbalTimerPrescaler;

void gbal_timer_start(GbalTimer timer, uint16_t reload, GbalTimerPrescaler prescaler);
void gbal_timer_stop(GbalTimer timer);
uint16_t gbal_timer_read(GbalTimer timer);
void gbal_timer_set_handler(GbalTimer timer, GbalIrqHandler handler);

#endif // GBAL_TIMER_H