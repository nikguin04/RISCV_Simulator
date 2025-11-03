#ifndef INSTRUCTION_FORWARD_H
#define INSTRUCTION_FORWARD_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
void handle_r_type(uint8_t rd, uint8_t funct3, uint8_t rs1, uint8_t rs2, uint8_t funct7);
void handle_i_type(uint8_t rd, uint8_t funct3, uint8_t rs1, int16_t imm);
void handle_s_type(uint8_t ls_imm, uint8_t funct3, uint8_t rs1, uint8_t rs2, uint8_t ms_imm); // Least significant and most significant immediates
void handle_b_type(uint32_t instruction, uint8_t funct3, uint8_t rs1, uint8_t rs2);
void handle_u_type(uint8_t rd, int32_t imm, bool opcode_bit_5);
void handle_j_type(uint8_t rd, uint32_t imm);

void handle_syscall(bool bit20);

#endif
