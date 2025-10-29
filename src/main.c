#include <executer.h>

#define TEST "./tests/task2/branchcnt"
#define TEST_FILE TEST ".bin"
#define TEST_RESULTFILE TEST ".res"




int main(int argc, char *argv[]) {
	execute_program(TEST_FILE);
	return 0;
}
