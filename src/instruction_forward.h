#ifndef INSTRUCTION_FORWARD_H
#define INSTRUCTION_FORWARD_H

#include "instruction_decoding.h"

exec_op(instruction_t instruction);
exec_load(instruction_t instruction);
exec_jalr(instruction_t instruction);
exec_system(instruction_t instruction);
exec_op_imm(instruction_t instruction);
exec_store(instruction_t instruction);
exec_branch(instruction_t instruction);
exec_auipc(instruction_t instruction);
exec_lui(instruction_t instruction);
exec_jal(instruction_t instruction);

#endif
