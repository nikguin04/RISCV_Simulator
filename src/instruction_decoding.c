#include <stdio.h>
#include <stdlib.h>
#include "instruction_decoding.h"

[[noreturn]] void invalidInstruction(uint32_t instruction) {
	fprintf(stderr, "Invalid instruction: %08X", instruction);
	exit(1);
}

instruction_t decodeInstruction(uint32_t instruction) {
	int opcode = BITS(instruction, 0, 6);
	int rd = BITS(instruction, 7, 11);
	int funct3 = BITS(instruction, 12, 14);
	int rs1 = BITS(instruction, 15, 19);
	int rs2 = BITS(instruction, 20, 24);
	int funct7 = BITS(instruction, 25, 31);
	uint32_t imm = 0;
	// The lowest two bits are 11 for all valid instructions
	if (instruction & 0b11 != 0b11) invalidInstruction(instruction);
	switch (opcode >> 2) {
	case 0b01100: // OP
		// R-type
		break;
	case 0b00000: // LOAD
	case 0b11001: // JALR
	case 0b11100: // SYSTEM
	case 0b00100: // OP-IMM
		// I-type
		imm = (funct7 << 5) | rs2;
		break;
	case 0b01000: // STORE
		// S-type
		imm = (funct7 << 5) | rd;
		break;
	case 0b11000: // BRANCH
		// B-type
		imm = (funct7 << 5) | rd;
		imm = (imm & ~(1 | 1 << 11)) | ((imm & 1) << 11) | ((imm & (1 << 11)) << 1);
		break;
	case 0b00101: // AUIPC
	case 0b01101: // LUI
		// U-type
		imm = instruction & ~BIT_MASK(12);
		break;
	case 0b11011: // JAL
		// J-type
		imm = BITS(instruction, 21, 30) << 1;
		imm |= BITS(instruction, 20, 20) << 11;
		imm |= BITS(instruction, 31, 31) << 20;
		imm |= instruction & (BIT_MASK(20) ^ BIT_MASK(12));
		break;
	default: // Extension, reserved, or custom opcode
		invalidInstruction(instruction);
	}
	return (instruction_t) { opcode, rd, rs1, rs2, funct3, funct7, imm };
}
