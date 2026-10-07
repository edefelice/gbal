#ifndef GBAL_MEM_H
#define GBAL_MEM_H

#include <stdint.h>

// Copy 2 bytes at a time: destination and source must be 2 bytes aligned (checked via GBAL_ASSERT in debug builds).
// Source and Destination must not overlap (undefined behaviour).
void gbal_memcpy16(volatile void *destination, const volatile void *source, uint32_t halfword_count);
// Copy 4 bytes at a time: destination and source must be 4 bytes aligned (checked via GBAL_ASSERT in debug builds).
// Source and Destination must not overlap (undefined behaviour).
void gbal_memcpy32(volatile void *destination, const volatile void *source, uint32_t word_count);
// Fills halfword_count halfwords with value. Destination must be 2 bytes aligned (checked via GBAL_ASSERT in debug builds).
void gbal_memset16(volatile void *destination, const uint16_t value, uint32_t halfword_count);
// Fills word_count words with value. Destination must be 4 bytes aligned (checked via GBAL_ASSERT in debug builds).
void gbal_memset32(volatile void *destination, const uint32_t value, uint32_t word_count);

#endif // GBAL_MEM_H