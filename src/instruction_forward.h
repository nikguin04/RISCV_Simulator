#ifndef INSTRUCTION_FORWARD_H
#define INSTRUCTION_FORWARD_H

#include <stdint.h>
#include "memory.h"
void handle_b_type(uint32_t instruction, uint8_t funct3, uint8_t rs1, uint8_t rs2);

#endif
