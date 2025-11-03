#include <stdio.h>
#include <stdlib.h>
#include "executor.h"

int main(int argc, char const *argv[]) {
	char bin[256];
	sprintf(bin, "%s.bin", argv[1]);
	char res[256];
	sprintf(res, "%s.res", argv[1]);
	// printf("%s - %s", bin, res);
	executeProgram(bin);

	FILE *file = fopen(res, "rb");
	fseek(file, 0L, SEEK_END);
	long size = ftell(file); // Seek to end and get position
	fseek(file, 0L, SEEK_SET); // Rewind to start
	int32_t *expected;
	expected = malloc(size); // We assume file is always 32 bit aligned!
	fread(expected, 1, size, file); // For now assume no errors and that everything is read at once

	for (int i = 0; i < size / 4; i++) {
		if (registers[i] != expected[i]) {
			printf("Register %d, are not as expected %08X - %08X (actual - expected)", i, registers[i], expected[i]);
			return 1;
		}
	}

	return 0;
}
