#ifndef GBAL_DEBUG_H
#define GBAL_DEBUG_H

#include <stdint.h>
#include "gbal/hardware.h"

static inline void gbal_assert_fail(void) {
    GBAL_REG_IME = 0; // Stop IRQ from interfering
    GBAL_REG_DISPCNT = 0x0403; // Mode 3 + BG2
    volatile uint16_t *vram = GBAL_MEM_VRAM;
    for (int i = 0; i < GBAL_SCREEN_W * GBAL_SCREEN_H; i++) {
        vram[i] = 0x001F; // red screen
    }
    while(1); // freeze
}

#ifdef GBAL_DEBUG
    #define GBAL_ASSERT(cond) do { if (!(cond)) gbal_assert_fail(); } while(0)
#else
    #define GBAL_ASSERT(cond) ((void)(cond))
#endif

#endif // GBAL_DEBUG_H