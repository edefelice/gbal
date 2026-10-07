#ifndef GBAL_IRQ_H
#define GBAL_IRQ_H

#include <stdint.h>

typedef void (*GbalIrqHandler)(void);
typedef enum GbalIrqSource {
    GBAL_IRQ_VBLANK = 0,
    GBAL_IRQ_HBLANK,
    GBAL_IRQ_VCOUNT,
    GBAL_IRQ_TIMER0,
    GBAL_IRQ_TIMER1,
    GBAL_IRQ_TIMER2,
    GBAL_IRQ_TIMER3,
    GBAL_IRQ_SERIAL,
    GBAL_IRQ_DMA0,
    GBAL_IRQ_DMA1,
    GBAL_IRQ_DMA2,
    GBAL_IRQ_DMA3,
    GBAL_IRQ_KEYPAD,
    GBAL_IRQ_GAME_PAK,
    GBAL_IRQ_COUNT
} GbalIrqSource;

void gbal_irq_init(void);
void gbal_irq_register(GbalIrqSource source, GbalIrqHandler handler);
void gbal_irq_vblank_init(void);
void gbal_wait_vblank(void);
uint32_t gbal_irq_vblank_count(void);

#endif // GBAL_IRQ_H