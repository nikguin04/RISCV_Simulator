#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define TEST_FILE "./tests/task1/addlarge.bin"

static char memory[0x100000];
static int32_t registers[32];
static uint32_t *program;

void printBinary(unsigned int n, uint8_t len) {
	for (int i = len - 1; i >= 0; i--) {
		putchar((n & (1u << i)) ? '1' : '0');
	}
	putchar('\n');
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
	}

	// Print out all the register values
	for (int i = 0; i < 32; i++) {
		printf("x%d = %i (0x%X)", i, registers[i], registers[i]);
	}

	free(program);
	return 0;
}
