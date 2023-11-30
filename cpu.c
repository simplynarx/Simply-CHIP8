/*
 * cpu.c - 
 */

#include "cpu.h"

enum OPCODES{
	OP_EXCMACHSUB = 0x0000, 			//where 0x0000 == 0x0NNN, execute machine language subroutine at address NNN
	OP_CLRSCRN = 0x00E0, 					//clear screen
	OP_RTRN = 0x00EE, 						//return from subroutine 
	OP_JMP = 0x1000, 							//where 0x1000 == 0x1NNN, jump to address NNN
	OP_EXCSUB = 0x2000,						//where 0x2000 == 0x2NNN, execute subroutine at address NNN
	OP_SKIPIFEQL = 0x3000, 				//where 0x3000 == 0x3XNN, skip following instruction if value of register VX == NN
	OP_SKIPIFNOTEQL = 0x4000, 		//where 0x4000 == 0x4XNN, skip following instruction if value of register VX != NN
};

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

void decode_execute_opcode(uint16_t opcode){
	uint8_t n1, n2, n3, n4;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

	switch(n1){
		case 0x0:
			if(opcode == OP_CLRSCRN){
				printf("Executing Opcode 00E0: Clear screen\n.");
				break;
			}
			else if(opcode == OP_RTRN){
				printf("Execting Opcode 00EE: Return from subroutine\n");
				break;
			}
			else{
				printf("Executing Opcode 0NNN: Execute subroutine at address NNN\n");
				break;
			}
		break;

		default:
			printf("Unknown Opcode\n");
			break;
	}

}
