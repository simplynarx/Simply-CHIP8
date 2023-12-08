#pragma once

#include "common.h"

typedef struct{
	uint8_t v[16]; //cpu registers
	uint16_t i; //address register
	uint16_t pc; //program counter
	uint16_t sp; //stack pointer
	uint8_t delay; //delay timer
	uint8_t sound; //sound timer
} CPU;

void cycle_cpu(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, int display_pitch);
void decode_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, uint16_t opcode);
void execute_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, uint16_t instruction, uint16_t opcode); 

enum OPCODES{
//	OP_EXC_MACH_SUB = 0x0000, 												//Where 0x0000 == 0x0NNN, execute machine language subroutine at address NNN
	OP_CLR_SCRN = 0x00E0, 														//Clear screen
	OP_RTRN = 0x00EE, 																//Return from subroutine 
	OP_JMP = 0x1000, 																	//Where 0x1000 == 0x1NNN, jump to address NNN
	OP_EXC_SUB = 0x2000,															//Where 0x2000 == 0x2NNN, execute subroutine at address NNN
	OP_SKIP_IF_ADR_EQL = 0x3000, 											//Where 0x3000 == 0x3XNN, skip following instruction if value of register VX == NN
	OP_SKIP_IF_ADR_NOT_EQL = 0x4000, 									//Where 0x4000 == 0x4XNN, skip following instruction if value of register VX != NN
	OP_SKIP_IF_REG_EQL = 0x5000,											//Where 0x5000 == 0x5XY0, skip following instruction if value of register VX == VY
	OP_STORE_VAL_ONE_REG = 0x6000,										//Where 0x6000 == 0x6XNN, store value NN in register VX
	OP_ADD_VAL_ONE_REG = 0x7000, 											//Where 0x7000 == 0x7XNN, add the value of NN to register VX
	OP_STORE_VAL_TWO_REG = 0x8000, 										//Where 0x8000 == 0x8XY0, store the value of register VY in register VX
	OP_SET_BITWISE_OR = 0x8001, 											//Where 0x8001 == 0x8XY1, set VX to VX OR VY
	OP_SET_BITWISE_AND = 0x8002, 											//Where 0x8002 == 0x8XY2, set VX to VX AND VY
	OP_SET_BITWISE_XOR = 0x8003, 											//Where 0x8003 == 0x8XY3, set VX to VX XOR VY
	OP_ADD_VAL_TWO_REG = 0x8004, 											/*Where 0x8004 == 0x8XY4, add the value of VY to VX.
																											Set VF to 01 if carry occurs, otherwise set VF to 00*/
	OP_SUBTRC_TWO_REG = 0x8005, 											/*Where 0x8005 == 0x8XY5, subtract the value of VY from VX.
																											Set VF to 00 if borrow occurs, otherwise set VF to 01*/
	OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG = 0x8006, 			//Where 0x8006 == 0x8XY6, store the value of VY shifted right one bit in VX
	OP_SET_SUBTRC_TWO_REG = 0x8007, 									/*Where 0x8007 == 0x8XY7, set register VX to value of VY minus VX. 
																											Set VF to 00 if borrow occurs, otherwise set VF to 01*/
	OP_STORE_LEFT_SHIFTED_VAL_TWO_REG = 0x800E, 			//Where 0x800E == 0x8XYE, store the value of VY shifted left one bit in VX
	OP_SKIP_IF_REG_NOT_EQL = 0x9000, 									//Where 0x9000 == 0x9XY0, skip following instruction if value of VX != VY
	OP_STORE_ADR_IN_ADR_REG = 0xA000, 														//Where 0xA000 == 0xANNN, store memory address NNN in I (address register)
	OP_JMP_PLUS_REG = 0xB000, 												//Where 0xB000 == 0xBNNN, jump to address NNN + the value in V0
	OP_SET_RAND = 0xC000, 														//Where 0xC000 == 0xCXNN, set VX to a random number with a mask of NN
	OP_DRAW_SPRITE = 0xD000, 													/*Where 0xD000 == 0xDXYN, draw a sprite at position (VX, VY) with N bytes of 
																											sprite data starting at the address stored in I. Set VF to 01 if any set pixels 
																											are changed to unset, otherwise set VF to 00*/
	OP_SKIP_IF_KEY_PRESSED = 0xE09E, 									/*Where 0xE09E == 0xEX9E, skip following instruction if the key 
																											corresponding to the hex value currently stored in VX is pressed*/
	OP_SKIP_IF_KEY_NOT_PRESSED = 0xE0A1, 							/*where 0xE0A1 == 0xEXA1, skip following instruction if the key 
																											corresponding to the hex value currently stored in VX is not pressed*/
	OP_STORE_DELAY_VAL_IN_REG = 0xF007, 							//Where 0xF007 == 0xFX07, store the current value of the delay timer in register VX
	OP_STORE_KEYPRESS = 0xF00A, 											//Where 0xF00A == 0xFX0A, wait for a keypress and store it in register VX
	OP_SET_DELAY_VAL_FROM_REG = 0xF015, 							//Where 0xF015 == 0xFX15, set the delay timer to the value of register VX
	OP_SET_SOUND_VAL_FROM_REG = 0xF018, 							//Where 0xF018 == 0xFX18, set the sound timer to the value of register VX
	OP_ADD_TO_ADR_REG = 0xF01E, 											//Where 0xF01E == 0xFX1E, add the value stored in VX to I
	OP_SET_ADR_REG_FROM_REG = 0xF029, 								/*Where 0xF029 == 0xFX29, set I to the memory address of the sprite data corresponding
																											to the hex digit stored in VX*/
	OP_STORE_BINCODED_DEC = 0xF033, 									/*Where 0xF033 == 0xFX33, store the binary-coded decimal equivalent of the value
																											stored in VX at addresses I, I+1, and I+2*/
	OP_STORE_MULTI_REG_VALS = 0xF055, 								/*Where 0xF055 == 0xFX55, store the values of registers V0 to VX inclusive in memory,
																											starting at address I. I is set to I+X+1 after operation is complete*/
	OP_FILL_MULTI_REG_VALS = 0xF065, 									/*Where 0xF065 == 0xFX65, fill the values of registers V0 to VX inclusive with the
																											values stored in memory starting at address I. I is set to I+X+1 after operation
																											is complete*/
};
