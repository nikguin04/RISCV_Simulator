#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TEST_FILE "./tests/task1/addlarge.bin"


void printBinary(unsigned int n, uint8_t len) {
    for (int i = len-1; i >= 0; i--) {
        putchar((n & (1u << i)) ? '1' : '0');
    }
    putchar('\n');
}

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
        uint32_t instruction = program[i];
        printf("%08X\t", instruction);

        uint8_t opcode = instruction & 0b01111111;
        printBinary(opcode, 8);
    }

    return 0;
}
