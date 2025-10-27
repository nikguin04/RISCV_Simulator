#include "instruction_forward.h"
void handle_b_type(uint32_t instruction, uint8_t funct3, uint8_t rs1, uint8_t rs2) {
	switch (funct3) {
	case 000: // BEQ
		if (registers[rs1] == registers[rs2]) { break; } else { return; }
	case 001: // BNE
		if (registers[rs1] != registers[rs2]) { break; } else { return; }
	case 100: // BLT
		if (registers[rs1] < registers[rs2]) { break; } else { return; }
	case 101: // BGE
		if (registers[rs1] >= registers[rs2]) { break; } else { return; }
	case 110: // BLTU
		if ((uint32_t)registers[rs1] < (uint32_t)registers[rs2]) { break; } else { return; }
	case 111: // BGEU
		if ((uint32_t)registers[rs1] >= (uint32_t)registers[rs2]) { break; } else { return; }
	}

	uint16_t offset =
		(instruction & (1 << 31)) >> 19    // 31 bit = 12 offset bit
		| (BITS(instruction, 25, 30)) << 5 // 30-25 bits = 10-5 offset bit
		| (instruction & (1 << 7)) << 4    // 7 bit = 11 offset bit
		| (BITS(instruction, 8, 11)) << 1; // 11-8 bits = 1-4 offset bit
	pc += offset;
}



void handle_u_type(uint8_t rd, uint32_t imm, bool opcode_bit_5) {
	// LUI
	if (opcode_bit_5) {
		registers[rd] = imm << 12;
	}
	// later Add AUIPC



}

void handle_syscall(bool bit20) {

	if (bit20) {
		// EBREAK
	} else {
		// ECALL
		switch (registers[17]) {
		case 10:
			// Print out all the register values
			for (int i = 0; i < 32; i++) {
				printf("x%d = %i (0x%X)", i, registers[i], registers[i]);
			}
			exit(0);

		default:
			exit(1);
		}
	}
}
