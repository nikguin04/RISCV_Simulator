#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "instruction_execution.h"
#include "executor.h"
#include "memory.h"

bool halt = false;

[[noreturn]] void invalidFunct3(const char *opcode, uint8_t funct3) {
	fprintf(stderr, "Invalid funct3 for %s opcode: %3b", opcode, funct3);
	exit(1);
}

void exec_branch(instruction_t i) {
	switch (i.funct3) {
	case 0b000: // BEQ
		if (registers[i.rs1] == registers[i.rs2]) { break; } else { return; }
	case 0b001: // BNE
		if (registers[i.rs1] != registers[i.rs2]) { break; } else { return; }
	case 0b100: // BLT
		if (registers[i.rs1] < registers[i.rs2]) { break; } else { return; }
	case 0b101: // BGE
		if (registers[i.rs1] >= registers[i.rs2]) { break; } else { return; }
	case 0b110: // BLTU
		if ((uint32_t)registers[i.rs1] < (uint32_t)registers[i.rs2]) { break; } else { return; }
	case 0b111: // BGEU
		if ((uint32_t)registers[i.rs1] >= (uint32_t)registers[i.rs2]) { break; } else { return; }
	default:
		invalidFunct3("BRANCH", i.funct3);
	}

	pc += SIGN_EXT(i.imm, 13) - 4;
}


void exec_jal(instruction_t i) {
	registers[i.rd] = pc;
	pc += SIGN_EXT(i.imm, 21) - 4;
}


void exec_jalr(instruction_t i) {
	registers[i.rd] = pc;
	pc = (registers[i.rs1] + SIGN_EXT(i.imm, 12)) & ~1;
}


void exec_lui(instruction_t i) {
	registers[i.rd] = i.imm;
}


void exec_auipc(instruction_t i) {
	registers[i.rd] = pc + i.imm - 4;
}


void exec_op(instruction_t i) {
	switch (i.funct3) {
	case 0b000: // ADD / SUB
		bool sub = i.funct7;
		registers[i.rd] = sub ? registers[i.rs1] - registers[i.rs2] : registers[i.rs1] + registers[i.rs2];
		break;
	case 0b001: // SLL
		registers[i.rd] = registers[i.rs1] << registers[i.rs2];
		break;
	case 0b010: // SLT
		registers[i.rd] = registers[i.rs1] < registers[i.rs2] ? 1 : 0;
		break;
	case 0b011: // SLTU
		registers[i.rd] = (uint32_t)registers[i.rs1] < (uint32_t)registers[i.rs2] != 0 ? 1 : 0;
		break;
	case 0b100: // XOR
		registers[i.rd] = registers[i.rs1] ^ registers[i.rs2];
		break;
	case 0b101: // SRL / SRA
		bool sra = i.funct7;
		registers[i.rd] = sra ? registers[i.rs1] >> registers[i.rs2] : (uint32_t)registers[i.rs1] >> registers[i.rs2];
		break;
	case 0b110: // OR
		registers[i.rd] = registers[i.rs1] | registers[i.rs2];
		break;
	case 0b111: //AND
		registers[i.rd] = registers[i.rs1] & registers[i.rs2];
		break;
	default:
		invalidFunct3("OP", i.funct3);
	}
}


void exec_store(instruction_t i) {
	uint32_t address = registers[i.rs1] + SIGN_EXT(i.imm, 12);
	switch (i.funct3) {
	case 0b000: // SB
		*(int8_t *)(memory + address) = (int8_t)registers[i.rs2];
		break;
	case 0b001: // SH
		*(int16_t *)(memory + address) = (int16_t)registers[i.rs2];
		break;
	case 0b010: // SW
		*(int32_t *)(memory + address) = registers[i.rs2];
		break;
	default:
		invalidFunct3("STORE", i.funct3);
	}
}


void exec_op_imm(instruction_t i) {
	switch (i.funct3) {
	case 0b000: // ADDI
		registers[i.rd] = registers[i.rs1] + SIGN_EXT(i.imm, 12);
		break;
	case 0b001: // SLLI
		registers[i.rd] = registers[i.rs1] << i.imm;
		break;
	case 0b010: // SLTI
		registers[i.rd] = registers[i.rs1] < SIGN_EXT(i.imm, 12);
		break;
	case 0b011: // SLTIU
		registers[i.rd] = ((uint32_t)registers[i.rs1]) < i.imm;
		break;
	case 0b100: // XORI
		registers[i.rd] = registers[i.rs1] ^ i.imm;
		break;
	case 0b101: // SRLI / SRAI
		bool srai = i.funct7;
		registers[i.rd] = srai ? registers[i.rs1] >> i.imm : (uint32_t)registers[i.rs1] >> i.imm;
		break;
	case 0b110: // ORI
		registers[i.rd] = registers[i.rs1] | i.imm;
		break;
	case 0b111: // ANDI
		registers[i.rd] = registers[i.rs1] & i.imm;
		break;
	default:
		invalidFunct3("OP-IMM", i.funct3);
	}
}


void exec_load(instruction_t i) {
	uint32_t address = registers[i.rs1] + SIGN_EXT(i.imm, 12);
	switch (i.funct3) {
	case 0b000: // LB
		registers[i.rd] = *(int8_t *)(memory + address);
		break;
	case 0b001: // LH
		registers[i.rd] = *(int16_t *)(memory + address);
		break;
	case 0b010: // LW
		registers[i.rd] = *(int32_t *)(memory + address);
		break;
	case 0b100: // LBU
		registers[i.rd] = *(uint8_t *)(memory + address);
		break;
	case 0b101: // LHU
		registers[i.rd] = *(uint16_t *)(memory + address);
		break;
	default:
		invalidFunct3("LOAD", i.funct3);
	}
}


void exec_system(instruction_t i) {
	if (i.imm) {
		// EBREAK
		fputs("EBREAK instruction not supported!", stderr);
		exit(1);
	} else {
		// ECALL
		switch (registers[17]) {
		case 10:
			// Print out all the register values
			for (int i = 0; i < 32; i++) {
				//printf("x%d = %i (0x%X)\n", i, registers[i], registers[i]); // EX: x10 = -2147483647 (0x80000001)
				printf("%08X", registers[i]); // EX: 80000001
			}
			halt = true;
			break;
		default:
			exit(1);
		}
	}
}
