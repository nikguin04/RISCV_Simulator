#include <executer.h>

#define TEST "./tests/task1/addlarge"
#define TEST_FILE TEST ".bin"
#define TEST_RESULTFILE TEST ".res"




int main(int argc, char *argv[]) {
	execute_program(TEST_FILE);
	return 0;
}
