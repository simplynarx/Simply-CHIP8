/*
 * cpu.c - simulated CHIP-8 CPU and all functions associated with it
 */

#include "cpu.h"
#include "display.h"
#include "mem.h"
#include "stack.h"

/*
 * Function: 	initialize_cpu
 * -------------------------
 *
 * Initializes the CPU for use. 
 *
 * *cpu: the CPU
 */
void cycle_cpu(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, int display_pitch){
	uint16_t opcode;
	//cpu->pc = 0x200;

		//printf("Address %02hx: \t\n", cpu->pc);
		opcode = (mem[cpu->pc] << 8) | (mem[cpu->pc + 1] & 0xFF);
		decode_opcode(cpu, stack, video_mem, mem, opcode);

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
 * opcode: a CPU Opcode
 */
void decode_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, uint16_t opcode){
	switch((opcode >> 12)){
		case 0x0:
			switch(opcode){
				case OP_CLR_SCRN: printf("OP_CLR_SCRN\n"); execute_opcode(cpu, stack, video_mem, mem, OP_CLR_SCRN, opcode);break;
				case OP_RTRN: printf("OP_RTRN\n"); execute_opcode(cpu, stack, video_mem, mem, OP_RTRN, opcode); break;
				//default: printf("OP_EXC_MACH_SUB\n"); break;
			} break;
		case 0x1: printf ("OP_JMP\n"); execute_opcode(cpu, stack, video_mem, mem, OP_JMP, opcode); break;
		case 0x2: printf("OP_EXC_SUB\n"); execute_opcode(cpu, stack, video_mem, mem, OP_EXC_SUB, opcode); break;
		case 0x3: printf("OP_SKIP_IF_ADR_EQL\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SKIP_IF_ADR_EQL, opcode); break;
		case 0x4: printf("OP_SKIP_IF_ADR_NOT_EQL\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SKIP_IF_ADR_NOT_EQL, opcode); break;
		case 0x5: printf("OP_SKIP_IF_REG_EQL\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SKIP_IF_REG_EQL, opcode); break;
		case 0x6: printf("OP_STORE_VAL_ONE_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_STORE_VAL_ONE_REG, opcode); break;
		case 0x7: printf("OP_ADD_VAL_ONE_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_ADD_VAL_ONE_REG, opcode); break;
		case 0x8:
			switch(opcode & 0x000F){
				case 0x0: printf("OP_STORE_VAL_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_STORE_VAL_TWO_REG, opcode); break;
				case 0x1: printf("OP_SET_BITWISE_OR\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SET_BITWISE_OR, opcode); break;
				case 0x2: printf("OP_SET_BITWISE_AND\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SET_BITWISE_AND, opcode); break;
				case 0x3: printf("OP_SET_BITWISE_XOR\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SET_BITWISE_XOR, opcode); break;
				case 0x4: printf("OP_ADD_VAL_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_ADD_VAL_TWO_REG, opcode); break;
				case 0x5: printf("OP_SUBTRC_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SUBTRC_TWO_REG, opcode); break;
				case 0x6: printf("OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_STORE_RIGHT_SHIFTED_VAL_TWO_REG, opcode); break;
				case 0x7: printf("OP_SET_SUBTRC_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SET_SUBTRC_TWO_REG, opcode); break;
				case 0xE: printf("OP_STORE_LEFT_SHIFTED_VAL_TWO_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_STORE_LEFT_SHIFTED_VAL_TWO_REG, opcode); break;
				default: printf("Unknown Opcode\n"); break;
			} break;
		case 0x9: printf("OP_SKIP_IF_REG_NOT_EQL\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SKIP_IF_REG_NOT_EQL, opcode); break;
		case 0xA: printf("OP_STORE_ADR_IN_ADR_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_STORE_ADR_IN_ADR_REG, opcode); break;
		case 0xB: printf("OP_JMP_PLUS_REG\n"); execute_opcode(cpu, stack, video_mem, mem, OP_JMP_PLUS_REG, opcode); break;
		case 0xC: printf("OP_SET_RAND\n"); execute_opcode(cpu, stack, video_mem, mem, OP_SET_RAND, opcode); break;
		case 0xD: printf("OP_DRAW_SPRITE\n"); execute_opcode(cpu, stack, video_mem, mem, OP_DRAW_SPRITE, opcode); break;
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
 * instruction: the instruction to be executed
 * 							by the CPU
 * opcode: a CPU opcode
 *
 */
void execute_opcode(CPU *cpu, uint16_t *stack, uint32_t *video_mem, uint8_t *mem, uint16_t instruction, uint16_t opcode){
	uint8_t n1, n2, n3, n4;
	uint16_t sum, diff, shift;
	uint8_t x_pos, y_pos, height, sprite_byte, sprite_pixel;
	uint32_t *screen_pixel;

	srandom(time(NULL));
	uint8_t rand_num = (random() % CHAR_MAX);

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
			//cpu->v[n2] = cpu->v[n3];
			//shift = (cpu->v[n2] >> 4);
			cpu->v[n2] = (cpu->v[n2] >> 1);

			//if(shift == 0x1) cpu->v[0xF] = 0x1;
			//else if(shift == 0x0) cpu->v[0xF] = 0x0;
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
			/*cpu->v[n2] = cpu->v[n3];
			shift = (cpu->v[n2] << 8);
			cpu->v[n2] = (cpu->v[n2] << 8);

			if(shift == 0x1) cpu->v[0xF] = 0x1;
			else if(shift == 0x0) cpu->v[0xF] = 0x0;

			cpu->pc += 0x2;*/

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
	}
}
