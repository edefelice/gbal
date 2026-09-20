#include <stdint.h>
#include "gbal/hardware.h"
#include "gbal/irq.h"

#define DSTAT_VBLANK_IRQ_ENABLE (1 << 3)
#define IRQ_VBLANK 1

typedef void (*FunctionPtr)(void);
#define BIOS_ISR_VECTOR (*(volatile FunctionPtr*)(0x03007FFC))
#define BIOS_IF_MIRROR (*(volatile uint16_t *)0x03007FF8)

static volatile uint32_t vblank_count = 0;

static void __attribute__((target("arm"))) vblank_isr(void) {
    vblank_count++;
    GBAL_REG_IF = IRQ_VBLANK; // Confirmation: write 1 to swtich the flag off.
    BIOS_IF_MIRROR |= IRQ_VBLANK;
}

void gbal_irq_vblank_init(void) {
    GBAL_REG_IME = 0;
    BIOS_ISR_VECTOR = vblank_isr;
    GBAL_REG_IE |= IRQ_VBLANK;
    GBAL_REG_DISPSTAT |= DSTAT_VBLANK_IRQ_ENABLE;
    GBAL_REG_IME = 1;
}

uint32_t gbal_irq_vblank_count(void) { return vblank_count; }

void gbal_wait_vblank(void) {
    uint32_t start = vblank_count;
    while(vblank_count == start); // busy wait
}