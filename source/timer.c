#include <stdint.h>
#include "gbal/hardware.h"
#include "gbal/irq.h"
#include "gbal/timer.h"

#define TIMER_CASCADE_ENABLE (1 << 2)
#define TIMER_IRQ_ENABLE (1 << 6)
#define TIMER_ENABLE     (1 << 7)

// *_L is the reload when written and the current counter when read: the same address has 2 meanings.
static volatile uint16_t * const timer_counter[GBAL_TIMER_COUNT] = {
    &GBAL_REG_TM0CNT_L,
    &GBAL_REG_TM1CNT_L,
    &GBAL_REG_TM2CNT_L,
    &GBAL_REG_TM3CNT_L
};

static volatile uint16_t * const timer_control[GBAL_TIMER_COUNT] = {
    &GBAL_REG_TM0CNT_H,
    &GBAL_REG_TM1CNT_H,
    &GBAL_REG_TM2CNT_H,
    &GBAL_REG_TM3CNT_H
};

// The reload is latched into the counter when TIMER_ENABLE goes 0 -> 1, so we clear TIMER_ENABLE first.
void gbal_timer_start(GbalTimer timer, uint16_t reload, GbalTimerPrescaler prescaler) {
    if (timer < GBAL_TIMER_COUNT) {
        // Keep the IRQ-enable bit set by gbal_timer_set_handler: the full-register write
        // below would otherwise clear it and the timer would never fire.
        uint16_t timer_irq_bit = (*timer_control[timer] & (TIMER_IRQ_ENABLE));
        *timer_control[timer] &= ~(TIMER_ENABLE);
        *timer_counter[timer] = reload;
        *timer_control[timer] = prescaler | timer_irq_bit | (TIMER_ENABLE);
    }
}

void gbal_timer_stop(GbalTimer timer) {
    if (timer < GBAL_TIMER_COUNT) {
        *timer_control[timer] &= ~(TIMER_ENABLE); 
    }
}

uint16_t gbal_timer_read(GbalTimer timer) {
    if (timer < GBAL_TIMER_COUNT) {
        return *timer_counter[timer];
    }
    else {
        return 0;
    }
}

void gbal_timer_set_handler(GbalTimer timer, GbalIrqHandler handler) {
    if (timer < GBAL_TIMER_COUNT) {
        gbal_irq_register((GbalIrqSource)(GBAL_IRQ_TIMER0 + timer), handler);
        if (handler) {
            *timer_control[timer] |= (TIMER_IRQ_ENABLE);
        }
        else {
            *timer_control[timer] &= ~(TIMER_IRQ_ENABLE);
        }
    }
}

void gbal_timer_start_cascade(GbalTimer timer, uint16_t reload) {
    if (timer > GBAL_TIMER0 && timer < GBAL_TIMER_COUNT) {
        // Same as gbal_timer_start.
        uint16_t timer_irq_bit = (*timer_control[timer] & (TIMER_IRQ_ENABLE));
        *timer_control[timer] &= ~(TIMER_ENABLE);
        *timer_counter[timer] = reload;
        *timer_control[timer] = (TIMER_CASCADE_ENABLE) | (TIMER_ENABLE) | timer_irq_bit;
    }
}

// Start the cascade (high) timer first: it only counts overflows of the low timer after it is enabled,
// so starting it second would lose the first overflows.
void gbal_timer_start32(GbalTimer low) {
    if (low < GBAL_TIMER3) {
        gbal_timer_start_cascade(low + 1, 0);
        gbal_timer_start(low, 0, GBAL_TIMER_DIV_1);
    }
}

// The low timer can overflow between the reads: re-read the high half and retry if it changed.
uint32_t gbal_timer_read32(GbalTimer low) {
    if (low < GBAL_TIMER3) {
        uint16_t timer_high = 0;
        uint16_t timer_high_again = 0;
        uint16_t timer_low = 0;
        do {
            timer_high = gbal_timer_read(low + 1);
            timer_low = gbal_timer_read(low);
            timer_high_again = gbal_timer_read(low + 1);
        }
        while (timer_high != timer_high_again);
        return ((uint32_t)timer_high_again << 16) | timer_low;
    }
    return 0;
}