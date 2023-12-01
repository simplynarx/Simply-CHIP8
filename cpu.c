/*
 * cpu.c - 
 */

#include "cpu.h"
#include <stdint.h>
#include <stdio.h>

typedef struct{
	uint8_t v0, v1, v2, v3, v4, v5, v6, v7, 
					v8, v9, va, vb, vc, vd, ve, vf;
} REGISTERS;

typedef struct{
	REGISTERS reg; //cpu registers
	uint16_t i; //address register
	uint16_t pc; //program counter
	uint16_t sp; //stack pointer
	uint8_t mem[4096]; //4kb of memory
} CPU;

static CPU cpu;

void initialize_cpu(){
	FILE *fp = fopen("font.txt", "r");
	uint16_t i = 0x050;
//	while(fscanf(fp, "%hhx, %hhx, %hhx, %hhx, %hhx\n", &cpu.mem[i], &cpu.mem[i++], &cpu.mem[i++], &cpu.mem[i++], &cpu.mem[i++]) == 5){

//	}
}

/**
 * Decodes the opcode, determining which opcode should be executed
 */
void decode_opcode(uint16_t opcode){
	switch((opcode >> 12)){
		case 0x0:
			switch(opcode){
				case OP_CLR_SCRN: printf("OP_CLR_SCRN\n"); break;
				case OP_RTRN: printf("OP_RTRN\n"); break;
				default: printf("OP_EXC_MACH_SUB\n"); break;
			} break;
		case 0x1: printf("OP_JMP\n"); break;
		case 0x2: printf("OP_EXC_SUB\n"); break;
		case 0x3: printf("OP_SKIP_IF_ADR_EQL\n"); break;
		case 0x4: printf("OP_SKIP_IF_ADR_NOT_EQL\n"); break;
		case 0x5: printf("OP_SKIP_IF_REG_EQL\n"); break;
		case 0x6: printf("OP_STORE_VAL_ONE_REG\n"); break;
		case 0x7: printf("OP_ADD_VAL_ONE_REG\n"); break;
		case 0x8:
			switch(opcode & 0x000F){
				case 0x0: printf("OP_STORE_VAL_TWO_REG\n"); break;
				case 0x1: printf("OP_SET_BITWISE_OR\n"); break;
				case 0x2: printf("OP_SET_BITWISE_AND\n"); break;
				case 0x3: printf("OP_SET_BITWISE_XOR\n"); break;
				case 0x4: printf("OP_ADD_VAL_TWO_REG\n"); break;
				case 0x5: printf("OP_SUBTRC_TWO_REG\n"); break;
				case 0x6: printf("OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG\n"); break;
				case 0x7: printf("OP_SET_SUBTRC_TWO_REG\n"); break;
				case 0xE: printf("OP_STORE_LEFT_SHIFTED_VAL_TWO_REG\n"); break;
				default: printf("Unknown Opcode\n"); break;
			} break;
		case 0x9: printf("OP_SKIP_IF_REG_NOT_EQL\n"); break;
		case 0xA: printf("OP_STORE_ADR\n"); break;
		case 0xB: printf("OP_JMP_PLUS_REG\n"); break;
		case 0xC: printf("OP_SET_RAND\n"); break;
		case 0xD: printf("OP_DRAW_SPRITE\n"); break;
		case 0xE:
			switch(opcode & 0x00FF){
				case 0x9E: printf("OP_SKIP_IF_KEY_PRESSED\n"); break;
				case 0xA1: printf("OP_SKIP_IF_KEY_NOT_PRESSED\n"); break;
				default: printf("Unknown Opcode\n"); break;
			} break;
		case 0xF:
			switch(opcode & 0x00FF){
				case 0x7: printf("OP_STORE_DELAY_VAL_IN_REG\n"); break;
				case 0xA: printf("OP_STORE_KEYPRESS\n"); break;
				case 0x15: printf("OP_SET_DELAY_VAL_FROM_REG\n"); break;
				case 0x18: printf("OP_SET_SOUND_VAL_FROM_REG\n"); break;
				case 0x1E: printf("OP_ADD_TO_ADR_REG\n"); break;
				case 0x29: printf("OP_SET_ADR_REG_FROM_REG\n"); break;
				case 0x33: printf("OP_STORE_BINCODED_DEC\n"); break;
				case 0x55: printf("OP_STORE_MULTI_REG_VALS\n"); break;
				case 0x65: printf("OP_FILL_MULTI_REG_VALS\n"); break;
				default: printf("Unknown Opcode\n"); break;
			} break;
		default:
			printf("Unknown Opcode\n"); break;
	}
}

void execute_opcode(uint16_t opcode){
	uint8_t n1, n2, n3, n4;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

}
