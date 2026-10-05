#include <stdint.h>
#include "gbal/hardware.h"
#include "gbal/irq.h"

#define DSTAT_VBLANK_IRQ_ENABLE (1 << 3)

// The BIOS calls the function whose address is stored here on every IRQ.
// There is a single slot, so only irq.c is allowed to write it.
#define BIOS_ISR_VECTOR (*(volatile GbalIrqHandler*)(0x03007FFC))
// Flags read by the BIOS (IntrWait/VBlankIntrWait) to know which IRQs occurred.
// Whoever acknowledges IF must also set the same bits here.
#define BIOS_IF_MIRROR (*(volatile uint16_t *)0x03007FF8)

static volatile uint32_t vblank_count = 0;
// One handler per IF bit, indexed by GbalIrqSource. NULL = no handler.
static GbalIrqHandler handlers[GBAL_IRQ_COUNT];

// Handlers run with IRQs masked (no nesting): keep them short and do the real work
// in the main loop.
static void vblank_isr(void) {
    vblank_count++;
}

/*
  Entry point installed in the BIOS vector. Must be compiled as ARM (the BIOS jumps here in ARM state).

  'fired' is IF & IE: only sources we actually enabled.
  Ack happens BEFORE calling the handlers, so an event that re-triggers while a handler is running
  is not lost.
  The BIOS mirror is updated here so IntrWait/VblankIntrWait can see it.
*/
static void __attribute__((target("arm"))) irq_dispatcher(void) {
    uint32_t fired = GBAL_REG_IF & GBAL_REG_IE;
    GBAL_REG_IF = fired;
    BIOS_IF_MIRROR |= fired;
    for (int i = 0; i < GBAL_IRQ_COUNT; i++) {
        if ( (fired & (1 << i)) && handlers[i]) {
            handlers[i]();
        } 
    }
}

/*
  Installs the dispatcher in the BIOS vector. Call once at startup,
  before registering any handler. IME is switched on last, so no IRQ
  can fire before the vector is set.
*/
void gbal_irq_init(void) {
    GBAL_REG_IME = 0;
    BIOS_ISR_VECTOR = irq_dispatcher;
    GBAL_REG_IME = 1;
}

/*
  Sets (or removes, if handler is NULL) the handler for a source and toggles its bit in IE.
  IE is owned by this function: modules must NOT touch it.
  IME is saved and restored, not forced to 1, so this is safe to call even from inside a handler.
*/
void gbal_irq_register(GbalIrqSource source, GbalIrqHandler handler) {
    if (source < GBAL_IRQ_COUNT) {
        uint32_t reg_ime = GBAL_REG_IME;
        GBAL_REG_IME = 0;
        handlers[source] = handler;
        if (handler) {
            GBAL_REG_IE |= (1 << source);
        }
        else {
            GBAL_REG_IE &= ~(1 << source);
        }
        GBAL_REG_IME = reg_ime;
    }
}

// Enables the VBlank request in DISPSTAT (the source side) and registers the handler.
// IE and IME are handled by gbal_irq_register.
void gbal_irq_vblank_init(void) {
    GBAL_REG_DISPSTAT |= DSTAT_VBLANK_IRQ_ENABLE;
    gbal_irq_register(GBAL_IRQ_VBLANK, vblank_isr);
}

uint32_t gbal_irq_vblank_count(void) { return vblank_count; }

void gbal_wait_vblank(void) {
    uint32_t start = vblank_count;
    while(vblank_count == start); // busy wait
}