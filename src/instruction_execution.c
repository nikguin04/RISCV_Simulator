#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "instruction_execution.h"
#include "executor.h"
#include "memory.h"

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
		exit(1);
	}

	pc += SIGN_EXT(i.imm, 13);
	pc -= 4; // TODO: Check how PC is incremented when executing i, this is a quick workaround for now
}


void exec_jal(instruction_t i) {
	// TODO
}


void exec_jalr(instruction_t i) {
	// TODO
}


void exec_lui(instruction_t i) {
	registers[i.rd] = i.imm;
}


void exec_auipc(instruction_t i) {
	// TODO
}


void exec_op(instruction_t i) {
	switch (i.funct3) {
	case 000: // ADD / SUB
		bool sub = i.funct7;
		registers[i.rd] = sub ? registers[i.rs1] - registers[i.rs2] : registers[i.rs1] + registers[i.rs2];
		return;
	case 001: // SLL
		registers[i.rd] = registers[i.rs1] << registers[i.rs2];
		break;
	case 010: // SLT
		registers[i.rd] = registers[i.rs1] < registers[i.rs2] ? 1 : 0;
		break;
	case 011: // SLTU
		registers[i.rd] = (uint32_t)registers[i.rs1] < (uint32_t)registers[i.rs2] != 0 ? 1 : 0;
		break;
	case 100: // XOR
		registers[i.rd] = registers[i.rs1] ^ registers[i.rs2];
		break;
	case 101: // SRL / SRA
		bool sra = i.funct7;
		registers[i.rd] = sra ? registers[i.rs1] >> registers[i.rs2] : (uint32_t)registers[i.rs1] >> registers[i.rs2];
		break;
	case 110: // OR
		registers[i.rd] = registers[i.rs1] | registers[i.rs2];
		break;
	case 111: //AND
		registers[i.rd] = registers[i.rs1] & registers[i.rs2];
		break;
	default:
		exit(1);
	}
}


void exec_store(instruction_t i) {
	uint32_t address = i.rs1 + SIGN_EXT(i.imm, 12);
	switch (i.funct3) {
	case 0b000: // SB
		*(int8_t *)(memory + address) = (int8_t)registers[i.rs2];
		return;
	case 0b001: // SH
		*(int16_t *)(memory + address) = (int16_t)registers[i.rs2];
		return;
	case 0b010: // SW
		*(int32_t *)(memory + address) = registers[i.rs2];
		return;
	default:
		break;
	}
}


void exec_op_imm(instruction_t i) {
	switch (i.funct3) {
	case 0b000: // ADDI
		registers[i.rd] = registers[i.rs1] + SIGN_EXT(i.imm, 12);
		return;
	case 0b010: // SLTI
		registers[i.rd] = registers[i.rs1] < SIGN_EXT(i.imm, 12);
		return;
	case 0b011: // SLTIU
		registers[i.rd] = ((uint32_t)registers[i.rs1]) < i.imm;
		return;
	case 0b100: // XORI
		registers[i.rd] = registers[i.rs1] ^ i.imm;
		return;
	case 0b110: // ORI
		registers[i.rd] = registers[i.rs1] | i.imm;
		return;
	case 0b111: // ANDI
		registers[i.rd] = registers[i.rs1] & i.imm;
		return;
	default:
		break;
	}
}


void exec_load(instruction_t i) {
	uint32_t address = registers[i.rs1] + SIGN_EXT(i.imm, 12);
	switch (i.funct3) {
	case 0b000: // LB
		registers[i.rd] = *(int8_t *)(memory + address);
		return;
	case 0b001: // LH
		registers[i.rd] = *(int16_t *)(memory + address);
		return;
	case 0b010: // LW
		registers[i.rd] = *(int32_t *)(memory + address);
		return;
	case 0b100: // LBU
		registers[i.rd] = *(uint8_t *)(memory + address);
		return;
	case 0b101: // LHU
		registers[i.rd] = *(uint16_t *)(memory + address);
		return;
	default:
		break;
	}
}


void exec_system(instruction_t i) {
	if (i.imm) {
		// EBREAK
	} else {
		// ECALL
		switch (registers[17]) {
		case 10:
			// Print out all the register values
			for (int i = 0; i < 32; i++) {
				//printf("x%d = %i (0x%X)\n", i, registers[i], registers[i]); // EX: x10 = -2147483647 (0x80000001)
				printf("%08X", registers[i]); // EX: 80000001
			}
			exit(0);
		default:
			exit(1);
		}
	}
}
