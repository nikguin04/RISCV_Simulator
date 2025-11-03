#ifndef INSTRUCTION_DECODING_H
#define INSTRUCTION_DECODING_H

#define BIT_MASK(bits) ((1 << (bits)) - 1)
#define BITS(value, a, b) ((value >> a) & BIT_MASK(b - a + 1))

#include <stdint.h>
typedef struct {
	uint8_t opcode;
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;
	uint8_t funct3;
	uint8_t funct7;
	uint8_t imm;
} instruction_t;

instruction_t decodeInstruction(uint32_t instruction);

[[noreturn]] void invalidInstruction(uint32_t instruction);

#endif
