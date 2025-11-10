#include <stdio.h>
#include "executor.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fputs("Please specify a RISC-V flat binary file to execute!\n", stderr);
		return 1;
	}

	executeProgram(argv[1]);

	// Print out all the register values
	for (int i = 0; i < 32; i++) {
		printf("x%d = %i (0x%08X)\n", i, registers[i], registers[i]); // EX: x10 = -2147483647 (0x80000001)
	}

	return 0;
}
