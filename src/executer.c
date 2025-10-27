#include <executer.h>

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

	uint8_t opcode = BITS(instruction, 0, 6);
	uint8_t rd = BITS(instruction, 7, 11);
	uint8_t funct3 = BITS(instruction, 12, 14);
	uint8_t rs1 = BITS(instruction, 15, 19);
	uint8_t rs2 = BITS(instruction, 20, 24);
	uint8_t funct7 = BITS(instruction, 25, 31);
	int32_t imm = 0;
	if (instruction & 0b11 != 0b11) invalidInstruction(instruction);
	// The lowest two bits are 11 for all valid instructions
	switch (opcode >> 2) {
	case 0b01100: // OP
		// R-type
		handle_r_type(rd, funct3, rs1, rs2, funct7);
		break;
	case 0b00000: // LOAD
	case 0b11001: // JALR
	case 0b11100: // SYSTEM
		handle_syscall(instruction & (1 << 20));
		break;
	case 0b00100: // OP-IMM
		// I-type
		imm = ((funct7 & (1 << 6)) ? (BIT_MASK(21) << 11) : 0) | ((funct7 & BIT_MASK(6)) << 5) | rs2; // TODO: I dont think this handles signed ints
		handle_i_type(rd, funct3, rs1, imm);
		break;
	case 0b01000: // STORE
		// S-type
		imm = (funct7 << 5) | rd;
		break;
	case 0b11000: // BRANCH
		// B-type
		// imm = (funct7 << 5) | rd;
		// imm = (imm & ~(1 | 1 << 11)) | ((imm & 1) << 11) | ((imm & (1 << 11)) << 1);
		handle_b_type(instruction, funct3, rs1, rs2);
		break;
	case 0b00101: // AUIPC
	case 0b01101: // LUI
		// U-type
		imm = instruction & ~BIT_MASK(12);
		handle_u_type(rd, imm, instruction & (1 << 5));
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

void execute_program(const char *test_file) {
	FILE *file = fopen(test_file, "rb");
	fseek(file, 0L, SEEK_END);
	long size = ftell(file); // Seek to end and get position
	fseek(file, 0L, SEEK_SET); // Rewind to start
	// Boopstrapping: Program into memory
	//program = malloc(size); // We assume file is always 32 bit aligned!
	fread(memory, 1, size, file); // For now assume no errors and that everything is read at once

	while (true) {
		uint32_t *instruction = (uint32_t *)(memory + pc);
		pc += 4;
		executeInstruction(*instruction);
	}
}
