#ifndef MEMORY_H
#define MEMORY_H

#define BIT_MASK(bits) ((1 << (bits)) - 1)
#define BITS(value, a, b) ((value >> a) & BIT_MASK(b - a + 1))

#include <stdint.h>
extern char memory[];
extern int32_t registers[];
extern uint32_t pc;

#endif
