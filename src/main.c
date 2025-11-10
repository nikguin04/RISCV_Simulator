#include <stdio.h>
#include "executor.h"

#define TEST "./tests/task0/sblbtest"
#define TEST_FILE TEST ".bin"
#define TEST_RESULTFILE TEST ".res"

int main(int argc, char *argv[]) {
	executeProgram(TEST_FILE);

	// Print out all the register values
	for (int i = 0; i < 32; i++) {
		printf("x%d = %i (0x%08X)\n", i, registers[i], registers[i]); // EX: x10 = -2147483647 (0x80000001)
	}

	return 0;
}
