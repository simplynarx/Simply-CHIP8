/*
 * cpu.c - simulated CHIP-8 CPU and all functions associated with it
 */

#include "cpu.h"
#include "mem.h"

/*
 * Function: 	initialize_cpu
 * -------------------------
 *
 * Initializes the CPU for use. Stores the standard
 * CHIP-8 font in addresses 0x050 to 0x09F in the
 * interpreter area of memory (0x000 to 0x1FF)
 */
void initialize_cpu(CPU *cpu){

	cpu->pc = 0x200;

	//printf("PC before jump: %hx\n", cpu->pc);
	//decode_opcode(cpu, 0x1A22);
	//printf("PC after jump: %hx\n", cpu->pc);
	//printf("V2 before set: %hx\n", cpu->reg.v2);
	//decode_opcode(cpu, 0x602F);
	//printf("V2 after set: %hx\n", cpu->reg.v2);

}

/*
 * Function: 	decode_opcode
 * ------------------------
 *
 * Interprets and decodes an Opcode for execution
 * Helper function for execute_opcode
 *
 * opcode: a CPU Opcode
 *
 * returns: the name of the Opcode to be executed
 */
void decode_opcode(CPU *cpu, uint16_t opcode){
	switch((opcode >> 12)){
		case 0x0:
			switch(opcode){
				case OP_CLR_SCRN: printf("OP_CLR_SCRN\n"); break;
				case OP_RTRN: printf("OP_RTRN\n"); break;
				default: printf("OP_EXC_MACH_SUB\n"); break;
			} break;
		case 0x1: printf ("OP_JMP\n"); execute_opcode(cpu, OP_JMP, opcode); break;
		case 0x2: printf("OP_EXC_SUB\n"); break;
		case 0x3: printf("OP_SKIP_IF_ADR_EQL\n"); break;
		case 0x4: printf("OP_SKIP_IF_ADR_NOT_EQL\n"); break;
		case 0x5: printf("OP_SKIP_IF_REG_EQL\n"); break;
		case 0x6: printf("OP_STORE_VAL_ONE_REG\n"); execute_opcode(cpu, OP_STORE_VAL_ONE_REG, opcode); break;
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

/*
 * Function: 	execute_opcode
 * -------------------------
 *
 * Executes the decoded opcode
 */
void execute_opcode(CPU *cpu, uint16_t instruction, uint16_t opcode){
	uint8_t n1, n2, n3, n4;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

	switch(instruction){
		case OP_JMP:
			cpu->pc = (opcode & 0x0FFF);
			break;

	//	case OP_STORE_VAL_ONE_REG:
	//		V(2) = (opcode & 0x00FF);
	}
}
