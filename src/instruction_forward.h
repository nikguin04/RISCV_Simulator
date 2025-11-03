#ifndef INSTRUCTION_FORWARD_H
#define INSTRUCTION_FORWARD_H

#include "instruction_decoding.h"

void exec_op(instruction_t instruction);
void exec_load(instruction_t instruction);
void exec_jalr(instruction_t instruction);
void exec_system(instruction_t instruction);
void exec_op_imm(instruction_t instruction);
void exec_store(instruction_t instruction);
void exec_branch(instruction_t instruction);
void exec_auipc(instruction_t instruction);
void exec_lui(instruction_t instruction);
void exec_jal(instruction_t instruction);

#endif
