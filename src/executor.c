#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "executor.h"
#include <stdbool.h>

void printBinary(unsigned int n, uint8_t len) {
	for (int i = len - 1; i >= 0; i--) {
		putchar((n & (1u << i)) ? '1' : '0');
	}
	putchar('\n');
}

void executeInstruction(instruction_t instruction) {
	switch (instruction.opcode >> 2) {
	case 0b01100: // OP
		exec_op(instruction);
		break;
	case 0b00000: // LOAD
		exec_load(instruction);
		break;
	case 0b11001: // JALR
		exec_jalr(instruction);
		break;
	case 0b11100: // SYSTEM
		exec_system(instruction);
		break;
	case 0b00100: // OP-IMM
		exec_op_imm(instruction);
		break;
	case 0b01000: // STORE
		exec_store(instruction);
		break;
	case 0b11000: // BRANCH
		exec_branch(instruction);
		break;
	case 0b00101: // AUIPC
		exec_auipc(instruction);
		break;
	case 0b01101: // LUI
		exec_lui(instruction);
		break;
	case 0b11011: // JAL
		exec_jal(instruction);
		break;
	}
}

void executeProgram(const char *test_file) {
	FILE *file = fopen(test_file, "rb");
	if (file == NULL) {
		perror("Failed to open executable binary");
		exit(1);
	}
	fseek(file, 0L, SEEK_END);
	long size = ftell(file); // Seek to end and get position
	fseek(file, 0L, SEEK_SET); // Rewind to start
	// Read the entire program into memory
	size_t read = fread(memory, 1, size, file);
	fclose(file);
	if (read != size) {
		fprintf(stderr, "Failed to read entire program into memory, managed to read %d/%d bytes", read, size);
		exit(1);
	}

	while (!halt) {
		uint32_t instruction = *(uint32_t *)(memory + pc);
		instruction_t decoded = decodeInstruction(instruction);
		pc += 4;
		executeInstruction(decoded);
		registers[0] = 0;
	}
}
