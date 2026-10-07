#ifndef GBAL_TIMER_H
#define GBAL_TIMER_H

#include <stdint.h>
#include "gbal/irq.h"

typedef enum GbalTimer {
    GBAL_TIMER0 = 0,
    GBAL_TIMER1,
    GBAL_TIMER2,
    GBAL_TIMER3,
    GBAL_TIMER_COUNT
} GbalTimer;

typedef enum GbalTimerPrescaler {
    GBAL_TIMER_DIV_1 = 0,
    GBAL_TIMER_DIV_64,
    GBAL_TIMER_DIV_256,
    GBAL_TIMER_DIV_1024
} GbalTimerPrescaler;

// Each timer: 16-bit counter, reload value, prescaler (1/64/256/1024 of the 16.78 MHz CPU clock).
// On overflow it reloads and can raise an IRQ.

// Starts the timer from 'reload'. Preserves the IRQ-enable bit set by gbal_timer_set_handler,
// so the two calls can be made in any order.
void gbal_timer_start(GbalTimer timer, uint16_t reload, GbalTimerPrescaler prescaler);
void gbal_timer_stop(GbalTimer timer);
// Reads the CURRENT counter value (not the reload).
uint16_t gbal_timer_read(GbalTimer timer);
void gbal_timer_set_handler(GbalTimer timer, GbalIrqHandler handler);
// Counts overflows of timer-1 instead of clock ticks (prescaler ignored).
// Valid for timers 1..3. Start it BEFORE the timer feeding it.
void gbal_timer_start_cascade(GbalTimer timer, uint16_t reload);
// starts low (div 1) + low+1 (cascade): free-running 32-bit cycle counter.
// Both timers are busy while it runs. low must be 0..2.
void gbal_timer_start32(GbalTimer low);
// Reads that pair coherently. low must be 0..2.
uint32_t gbal_timer_read32(GbalTimer low);

#endif // GBAL_TIMER_H