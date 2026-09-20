#ifndef GBAL_IRQ_H
#define GBAL_IRQ_H

#include <stdint.h>

void gbal_irq_vblank_init(void);
void gbal_wait_vblank(void);
uint32_t gbal_irq_vblank_count(void);

#endif // GBAL_IRQ_H