/*
 * cpu.c - simulated CHIP-8 CPU and all functions associated with it
 */

#include "cpu.h"
#include "display.h"
#include "mem.h"
#include "stack.h"

/*
 * Function: 	cycle_cpu
 * -------------------------
 *
 * Simulates one full CPU cycle
 *
 * *cpu: the CPU
 * *stack: the CPU stack
 * *video_mem: the display
 * *mem: the address bus
 * *keypad: the CHIP-8 keypad
 * display_pitch: the number of bytes in a row of pixel data
 */
void cycle_cpu(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, int display_pitch){
	uint16_t opcode;

		opcode = (mem[cpu->pc] << 8) | (mem[cpu->pc + 1] & 0xFF);
		decode_opcode(cpu, stack, video_mem, mem, keypad, opcode);

	//decode_opcode(cpu, stack, opcode);
}

/*
 * Function: 	decode_opcode
 * ------------------------
 *
 * Interprets and decodes an Opcode for execution
 * Passes the decoded opcode into execute_opcode
 *
 * *cpu: the CPU
 * *stack: the CPU stack
 * *video_mem: the display
 * *mem: the address bus
 * *keypad: the CHIP-8 keypad
 * opcode: a CPU Opcode
 */
void decode_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, uint16_t opcode){
	switch((opcode >> 12)){
		case 0x0:
			switch(opcode){
				case OP_CLR_SCRN: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_CLR_SCRN, opcode);break;
				case OP_RTRN: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_RTRN, opcode); break;
			} break;
		case 0x1: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_JMP, opcode); break;
		case 0x2: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_EXC_SUB, opcode); break;
		case 0x3: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_ADR_EQL, opcode); break;
		case 0x4: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_ADR_NOT_EQL, opcode); break;
		case 0x5: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_REG_EQL, opcode); break;
		case 0x6: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_VAL_ONE_REG, opcode); break;
		case 0x7: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_ADD_VAL_ONE_REG, opcode); break;
		case 0x8:
			switch(opcode & 0x000F){
				case 0x0: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_VAL_TWO_REG, opcode); break;
				case 0x1: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_BITWISE_OR, opcode); break;
				case 0x2: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_BITWISE_AND, opcode); break;
				case 0x3: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_BITWISE_XOR, opcode); break;
				case 0x4: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_ADD_VAL_TWO_REG, opcode); break;
				case 0x5: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SUBTRC_TWO_REG, opcode); break;
				case 0x6: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG, opcode); break;
				case 0x7: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_SUBTRC_TWO_REG, opcode); break;
				case 0xE: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_LEFT_SHIFTED_VAL_TWO_REG, opcode); break;
				default: printf("Unknown Opcode\n"); exit(1); break;
			} break;
		case 0x9: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_REG_NOT_EQL, opcode); break;
		case 0xA: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_ADR_IN_ADR_REG, opcode); break;
		case 0xB: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_JMP_PLUS_REG, opcode); break;
		case 0xC: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_RAND, opcode); break;
		case 0xD: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_DRAW_SPRITE, opcode); break;
		case 0xE:
			switch(opcode & 0x00FF){
				case 0x9E: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_KEY_PRESSED, opcode); break;
				case 0xA1: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SKIP_IF_KEY_NOT_PRESSED, opcode); break;
				default: printf("Unknown Opcode\n"); exit(1); break;
			} break;
		case 0xF:
			switch(opcode & 0x00FF){
				case 0x7: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_DELAY_VAL_IN_REG, opcode); break;
				case 0xA: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_KEYPRESS, opcode); break;
				case 0x15: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_DELAY_VAL_FROM_REG, opcode); break;
				case 0x18: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_SOUND_VAL_FROM_REG, opcode); break;
				case 0x1E: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_ADD_TO_ADR_REG, opcode); break;
				case 0x29: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_SET_ADR_REG_FROM_REG, opcode); break;
				case 0x33: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_BINCODED_DEC, opcode); break;
				case 0x55: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_STORE_MULTI_REG_VALS, opcode); break;
				case 0x65: execute_opcode(cpu, stack, video_mem, mem, keypad, OP_FILL_MULTI_REG_VALS, opcode); break;
				default: printf("Unknown Opcode\n"); exit(1); break;
			} break;
		default:
			printf("Unknown Opcode\n"); exit(1); break;
	}
}

/*
 * Function: 	execute_opcode
 * -------------------------
 *
 * Executes the decoded opcode
 *
 * *cpu: the CPU
 * *stack: the CPU stack
 * *video_mem: the display
 * *mem: the address bus
 * *keypad: the CHIP-8 keypad
 * instruction: the instruction to be executed
 * 							by the CPU
 * opcode: a CPU opcode
 *
 */
void execute_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, bool *keypad, uint16_t instruction, uint16_t opcode){
	uint8_t n1, n2, n3, n4;
	uint16_t sum, diff, shift;
	uint8_t x_pos, y_pos, height, sprite_byte, sprite_pixel;
	uint32_t *screen_pixel;
	uint8_t key;
	uint8_t val;

	srand(time(NULL));
	uint8_t rand_num = rand() % 0xFF;

	n1 = (opcode & 0xF000) >> 12;
	n2 = (opcode & 0x0F00) >> 8;
	n3 = (opcode & 0x00F0) >> 4;
	n4 = (opcode & 0x000F);

	switch(instruction){

		case OP_CLR_SCRN:
			memset(&video_mem, 0, sizeof(video_mem));
			cpu->pc += 0x2;
			break;

		case OP_RTRN:
			cpu->pc = stack_pop(cpu, stack);
			cpu->pc += 0x2;
			break;

		case OP_JMP:
			cpu->pc = (opcode & 0x0FFF);
			break;

		case OP_EXC_SUB:
			stack_push(cpu, stack, cpu->pc);	
			cpu->pc = (opcode & 0x0FFF);
			break;

		case OP_SKIP_IF_ADR_EQL:
			if(cpu->v[n2] == (opcode & 0x00FF)) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_SKIP_IF_ADR_NOT_EQL:
			if(cpu->v[n2] != (opcode & 0x00FF)) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_SKIP_IF_REG_EQL:
			if(cpu->v[n2] == cpu->v[n3]) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_STORE_VAL_ONE_REG:
			cpu->v[n2] = (opcode & 0x00FF);
			cpu->pc += 0x2;
			break;

		case OP_ADD_VAL_ONE_REG:
			cpu->v[n2] += (opcode & 0x00FF);
			cpu->pc += 0x2;
			break;

		case OP_STORE_VAL_TWO_REG:
			cpu->v[n2] = cpu->v[n3];
			cpu->pc += 0x2;
			break;

		case OP_SET_BITWISE_OR:
			cpu->v[n2] = (cpu->v[n2] | cpu->v[n3]);
			cpu->pc += 0x2;
			break;

		case OP_SET_BITWISE_AND:
			cpu->v[n2] = (cpu->v[n2] & cpu->v[n3]);
			cpu->pc += 0x2;
			break;

		case OP_SET_BITWISE_XOR:
			cpu->v[n2] = (cpu->v[n2] ^ cpu->v[n3]);
			cpu->pc += 0x2;
			break;

		case OP_ADD_VAL_TWO_REG:
			sum = cpu->v[n2] + cpu->v[n3];
			if(sum > 0xFF) cpu->v[0xF] = 0x1;
			else cpu->v[0xF] = 0x0;

			cpu->v[n2] = sum & 0xFF;
			cpu->pc += 0x2;
			break;

		case OP_SUBTRC_TWO_REG:
			if(cpu->v[n2] > cpu->v[n3]) cpu->v[0xF] = 0x1;
			else cpu->v[0xF] = 0x0;

			cpu->v[n2] -= cpu->v[n3];
			cpu->pc += 0x2;
			break;

		case OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG:
			cpu->v[n2] = (cpu->v[n2] >> 1);
			cpu->v[0xF] = (cpu->v[n2] & 0x1);

			cpu->pc += 0x2;
			break;

		case OP_SET_SUBTRC_TWO_REG:
			if(cpu->v[n2] < cpu->v[n3]) cpu->v[0xF] = 0x1;
			else cpu->v[0xF] = 0x0;

			cpu->v[n2] = cpu->v[n3] - cpu->v[n2];
			cpu->pc += 0x2;
			break;

		case OP_STORE_LEFT_SHIFTED_VAL_TWO_REG:
			cpu->v[n2] = (cpu->v[n2] << 1);
			cpu->v[0xF] = (cpu->v[n2] & 0x1);
			cpu->pc += 0x2;
			break;


		case OP_SKIP_IF_REG_NOT_EQL:
			if(cpu->v[n2] != cpu->v[n3]) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_STORE_ADR_IN_ADR_REG:
			cpu->i = (opcode & 0x0FFF);
			cpu->pc += 0x2;
			break;

		case OP_JMP_PLUS_REG:
			cpu->pc = (opcode & 0x0FFF) + cpu->v[0x0];
			break;

		case OP_SET_RAND:
			cpu->v[n2] = rand_num & (opcode & 0x00FF);
			cpu->pc += 0x2;
			break;

		case OP_DRAW_SPRITE:
			height = n4;
			x_pos = cpu->v[n2] % DISPLAY_WIDTH;
			y_pos = cpu->v[n3] % DISPLAY_HEIGHT;
			cpu->v[0xF] = 0;

			for(uint32_t row = 0; row < height; ++row){
				sprite_byte = mem[cpu->i + row];

				for(uint32_t col = 0; col < 8; ++col){
					sprite_pixel = sprite_byte & (0x80 >> col);
					screen_pixel = &video_mem[(y_pos + row) * DISPLAY_WIDTH + (x_pos + col)];

					if(sprite_pixel){
						if(*screen_pixel == 0xFFFFFFFF){
							cpu->v[0xF] = 1;
						}

						*screen_pixel ^= 0xFFFFFFFF;
					}
				}
			}
			cpu->pc += 0x2;
			break;

		case OP_SKIP_IF_KEY_PRESSED:
			key = cpu->v[n2];
			if(keypad[key]) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_SKIP_IF_KEY_NOT_PRESSED:
			key = cpu->v[n2];
			if(!keypad[key]) cpu->pc += 0x4;
			else cpu->pc += 0x2;
			break;

		case OP_STORE_DELAY_VAL_IN_REG:
			cpu->v[n2] = cpu->delay;
			cpu->pc += 0x2;
			break;

		case OP_STORE_KEYPRESS:
			if(keypad[0]) cpu->v[n2] = 0;
			else if(keypad[1]) cpu->v[n2] = 1;
			else if(keypad[2]) cpu->v[n2] = 2;
			else if(keypad[3]) cpu->v[n2] = 3;
			else if(keypad[4]) cpu->v[n2] = 4;
			else if(keypad[5]) cpu->v[n2] = 5;
			else if(keypad[6]) cpu->v[n2] = 6;
			else if(keypad[7]) cpu->v[n2] = 7;
			else if(keypad[8]) cpu->v[n2] = 8;
			else if(keypad[9]) cpu->v[n2] = 9;
			else if(keypad[10]) cpu->v[n2] = 10;
			else if(keypad[11]) cpu->v[n2] = 11;
			else if(keypad[12]) cpu->v[n2] = 12;
			else if(keypad[13]) cpu->v[n2] = 13;
			else if(keypad[14]) cpu->v[n2] = 14;
			else if(keypad[15]) cpu->v[n2] = 15;
			else cpu->pc -= 0x2;

			cpu->pc += 0x2;
			break;

		case OP_SET_DELAY_VAL_FROM_REG:
			cpu->delay = cpu->v[n2];
			cpu->pc += 0x2;
			break;

		case OP_SET_SOUND_VAL_FROM_REG:
			cpu->sound = cpu->v[n2];
			cpu->pc += 0x2;
			break;
			
		case OP_ADD_TO_ADR_REG:
			cpu->i += cpu->v[n2];
			cpu->pc += 0x2;
			break;

		case OP_SET_ADR_REG_FROM_REG:
			cpu->i = 0x50 + (5 * cpu->v[n2]);
			cpu->pc += 0x2;
			break;

		case OP_STORE_BINCODED_DEC:
			val = cpu->v[n2];
			mem[cpu->i + 2] = val % 10;
			val /= 10;
			mem[cpu->i + 1] = val % 10;
			val /= 10;
			mem[cpu->i] = val % 10;
			
			cpu->pc += 0x2;
			break;

		case OP_STORE_MULTI_REG_VALS:
			for(uint8_t ii = 0; ii <= n2; ++ii)
				mem[cpu->i + ii] = cpu->v[ii];

			cpu->pc += 0x2;
			break;

		case OP_FILL_MULTI_REG_VALS:
			for(uint8_t ii = 0; ii <= n2; ++ii)
				cpu->v[ii] = mem[cpu->i + ii];

			cpu->pc += 0x2;
			break;

	}
}
