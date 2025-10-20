#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TEST_FILE "./tests/task1/addlarge.bin"

int main(int argc, char const *argv[]) {
    printf("Hello World\n");

    FILE *fptr;
    fptr = fopen(TEST_FILE, "r");
    fseek(fptr, 0L, SEEK_END);
    long size = ftell(fptr); // Seek to end and get position
    fseek(fptr, 0L, SEEK_SET); // Rewind to start
    uint32_t* program = malloc(size); // we assume file is always 32 bit alligned!
    fgets((char*)program, size, fptr);
    for (int i = 0; i < size/4; i++) { // DUMP HEX
        printf("%08X\t", program[i]);
    }

    return 0;
}
