#include <stdint.h>
#include "gbal/hardware.h"
#include "gbal/irq.h"
#include "gbal/timer.h"

#define TIMER_IRQ_ENABLE (1 << 6)
#define TIMER_ENABLE     (1 << 7)

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

void gbal_timer_start(GbalTimer timer, uint16_t reload, GbalTimerPrescaler prescaler) {
    if (timer < GBAL_TIMER_COUNT) {
        uint8_t timer_irq_bit = (*timer_control[timer] & (TIMER_IRQ_ENABLE));
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