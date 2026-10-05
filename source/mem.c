#include <stdint.h>
#include "gbal/mem.h"
#include "gbal/debug.h"

void gbal_memcpy16(volatile void *destination, const volatile void *source, uint32_t halfword_count) {
    GBAL_ASSERT(!(((uintptr_t)destination & 1u) || (((uintptr_t)source) & 1u))); // if either destination or source are not at least 2byte aligned

    volatile uint16_t *dst = (volatile uint16_t *)destination;
    const volatile uint16_t *src = (const volatile uint16_t *)source;
    while(halfword_count > 0) {
        *dst++ = *src++;
        halfword_count--;
    }
}

void gbal_memcpy32(volatile void *destination, const volatile void *source, uint32_t word_count) {
    GBAL_ASSERT(!(((uintptr_t)destination & 3u) || ((uintptr_t)source & 3u))); // if either destination or source are not at least 4byte aligned

    volatile uint32_t *dst = (volatile uint32_t *)destination;
    const volatile uint32_t *src = (const volatile uint32_t *)source;
    while(word_count > 0) {
        *dst++ = *src++;
        word_count--;
    }
}

void gbal_memset16(volatile void* destination, const uint16_t value, uint32_t halfword_count) {
    GBAL_ASSERT(!((uintptr_t)destination & 1u)); // if destination is not at least 2byte aligned.

    volatile uint16_t *dst = (volatile uint16_t *)destination;
    while(halfword_count > 0) {
        *dst++ = value;
        halfword_count--;
    }
}

void gbal_memset32(volatile void *destination, const uint32_t value, uint32_t word_count) {
    GBAL_ASSERT(!((uintptr_t)destination & 3u)); // if destination is not at least 4byte aligned.

    volatile uint32_t *dst = (volatile uint32_t *)destination;
    while(word_count > 0) {
        *dst++ = value;
        word_count--;
    }
}