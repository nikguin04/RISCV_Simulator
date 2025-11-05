#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "memory.h"
#include "instruction_decoding.h"
#include "instruction_execution.h"

#define SIGN_EXT(value, orig_size) (((int32_t)(value) << (32 - (orig_size))) >> (32 - (orig_size)))

void executeInstruction(instruction_t);
void executeProgram(const char *test_file);

#endif
