#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "memory.h"
#include "instruction_decoding.h"
#include "instruction_forward.h"

void executeInstruction(instruction_t);
void executeProgram(const char *test_file);

#endif
