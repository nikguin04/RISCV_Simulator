#include "instruction_forward.h"
void handle_b_type(uint32_t instruction, uint8_t funct3, uint8_t rs1, uint8_t rs2) {
	switch (funct3) {
	case 0b000: // BEQ
		if (registers[rs1] == registers[rs2]) { break; } else { return; }
	case 0b001: // BNE
		if (registers[rs1] != registers[rs2]) { break; } else { return; }
	case 0b100: // BLT
		if (registers[rs1] < registers[rs2]) { break; } else { return; }
	case 0b101: // BGE
		if (registers[rs1] >= registers[rs2]) { break; } else { return; }
	case 0b110: // BLTU
		if ((uint32_t)registers[rs1] < (uint32_t)registers[rs2]) { break; } else { return; }
	case 0b111: // BGEU
		if ((uint32_t)registers[rs1] >= (uint32_t)registers[rs2]) { break; } else { return; }
	default:
		exit(1);
	}

	int16_t offset =
		(instruction & (1 << 31)) >> 19    // 31 bit = 12 offset bit
		| (BITS(instruction, 25, 30)) << 5 // 30-25 bits = 10-5 offset bit
		| (instruction & (1 << 7)) << 4    // 7 bit = 11 offset bit
		| (BITS(instruction, 8, 11)) << 1 // 11-8 bits = 1-4 offset bit
		| (instruction & (1 << 31) ? BIT_MASK(3) << 13 : 0); // Extend 15-13 bits arithmetic
	pc += offset;
	pc -= 4; // TODO: Check how PC is incremented when executing instruction, this is a quick workaround for now
}

void handle_u_type(uint8_t rd, int32_t imm, bool opcode_bit_5) {
	// LUI
	if (opcode_bit_5) {
		registers[rd] = imm;
	}
	// later Add AUIPC

}

void handle_r_type(uint8_t rd, uint8_t funct3, uint8_t rs1, uint8_t rs2, uint8_t funct7) {
	switch (funct3) {
	case 000: // ADD / SUB
		registers[rd] = funct7 ? registers[rs1] - registers[rs2] : registers[rs1] + registers[rs2];
		return;
	default:
		break;
	}
}

void handle_i_type(uint8_t rd, uint8_t funct3, uint8_t rs1, int16_t imm) {
	switch (funct3) {
	case 000: // ADDI
		registers[rd] = registers[rs1] + imm;
		return;
	default:
		break;
	}
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
				//printf("x%d = %i (0x%X)\n", i, registers[i], registers[i]); // EX: x10 = -2147483647 (0x80000001)
				printf("%08X", registers[i]); // EX: 80000001
			}
			exit(0);

		default:
			exit(1);
		}
	}
}
