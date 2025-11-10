#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "executor.h"

int main(int argc, char *argv[]) {
	if (argc < 2) return 1;
	size_t len = strlen(argv[1]);
	char *fileName = malloc(len + 5);
	memcpy(fileName, argv[1], len);

	strcpy(fileName + len, ".bin");
	executeProgram(fileName);

	strcpy(fileName + len, ".res");
	int32_t expected[32];
	FILE *file = fopen(fileName, "rb");
	fread(expected, sizeof(int32_t), 32, file); // Assume no errors and that everything is read at once
	fclose(file);
	free(fileName);

	for (int i = 0; i < 32; i++) {
		if (registers[i] != expected[i]) {
			printf("Register %d: expected 0x%08X, got 0x%08X", i, expected[i], registers[i]);
			return 1;
		}
	}

	return 0;
}
