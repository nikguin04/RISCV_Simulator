#include "executor.h"

#define TEST "./tests/task0/sblbtest"
#define TEST_FILE TEST ".bin"
#define TEST_RESULTFILE TEST ".res"

int main(int argc, char *argv[]) {
	executeProgram(TEST_FILE);
	return 0;
}
