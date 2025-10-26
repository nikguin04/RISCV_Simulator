#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "memory.h"
#include "instruction_forward.h"

#define TEST_FILE "./tests/custom/btype_offsettest.bin"



#define BIT_MASK(bits) ((1 << (bits)) - 1)
#define BITS(value, a, b) ((value >> a) & BIT_MASK(b - a + 1))

void printBinary(unsigned int n, uint8_t len) {
	for (int i = len - 1; i >= 0; i--) {
		putchar((n & (1u << i)) ? '1' : '0');
	}
	putchar('\n');
}

[[noreturn]] void invalidInstruction(uint32_t instruction) {
	fprintf(stderr, "Invalid instruction: %08X", instruction);
	exit(1);
}

void executeInstruction(uint32_t instruction) {

	int opcode = BITS(instruction, 0, 6);
	int rd = BITS(instruction, 7, 11);
	int funct3 = BITS(instruction, 12, 14);
	int rs1 = BITS(instruction, 15, 19);
	int rs2 = BITS(instruction, 20, 24);
	int funct7 = BITS(instruction, 25, 31);
	int imm = 0;
	if (instruction & 0b11 != 0b11) invalidInstruction(instruction);
	// The lowest two bits are 11 for all valid instructions
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
		uint16_t offset =
			(instruction & (1 << 31) >> 19)    // 31 bit = 12 offset bit
			| (BITS(instruction, 25, 30) << 5) // 30-25 bits = 10-5 offset bit
			| (instruction & (1 << 7) << 4)    // 7 bit = 11 offset bit
			| (BITS(instruction, 8, 11) << 1); // 11-8 bits = 1-4 offset bit
		//handle_b_type(funct3, imm);
		break;
	case 0b00101: // AUIPC
	case 0b01101: // LUI
		// U-type
		imm = instruction & ~BIT_MASK(12);
		break;
	case 0b11011: // JAL
		// J-type
		imm = (funct7 << 5) | rs2;
		imm = (imm & ~(1 | 1 << 11)) | ((imm & 1) << 11) | (imm & (1 << 11) << (20 - 11));
		imm |= instruction & (BIT_MASK(19) ^ BIT_MASK(11));
		break;
	default: // Extension, reserved, or custom opcode
		invalidInstruction(instruction);
	}
}

int main(int argc, char *argv[]) {
	FILE *file = fopen(TEST_FILE, "rb");
	fseek(file, 0L, SEEK_END);
	long size = ftell(file); // Seek to end and get position
	fseek(file, 0L, SEEK_SET); // Rewind to start
	program = malloc(size); // We assume file is always 32 bit aligned!
	fread(program, 1, size, file); // For now assume no errors and that everything is read at once

	for (int i = 0; i < size / 4; i++) { // DUMP HEX
		uint32_t instruction = program[i];
		printf("%08X\t", instruction);

		uint8_t opcode = instruction & 0b01111111;
		printBinary(opcode, 8);

		executeInstruction(instruction);
	}

	// Print out all the register values
	for (int i = 0; i < 32; i++) {
		printf("x%d = %i (0x%X)", i, registers[i], registers[i]);
	}



	free(program);
	return 0;
}
