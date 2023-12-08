/***************************
 * CHIP8 RELATED FUNCTIONS *
 ***************************/

#include "chip8.h"
#include "instructions.h"

/*
 * init_chip8
 * ----------
 *
 * Initalizes the CHIP8 for use.
 *
 * *c8: a chip8_t struct
 */
void init_chip8(chip8_t *c8){
	c8->state = RUNNING; //Start CHIP8
	c8->pc = 0x200; //Set program counter to start of ROM

	/* Load commonly used font into memory */
	for(u8 i = FONT_START; i <= FONT_END; i += 0x1)
		c8->ram[i] = FONT_VAL[i - FONT_START];
}

/*
 * load_rom
 * --------
 *
 * Loads ROM into the CHIP8's RAM for use
 *
 * *c8: a chip8_t struct
 * *rom: the name of the ROM file
 *
 * returns: false is failed to open ROM file, true otherwise
 */
bool load_rom(chip8_t *c8, char *rom){
	FILE *fp;
	u32 size;
	u8 *data;

	fp = fopen(rom, "r");
	/* If the rom does not exist */
	if(fp == NULL){
		fprintf(stderr, "ERROR: File does not exist.\n");
		return false;
	} 

	/* Get ROM file size */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	/* Retrieve ROM file data */
	data = malloc(size);
	fread(data, size, 1, fp);
	fclose(fp);

	/* Load ROM file into RAM */
	for(u16 i = ROM_START; i < ROM_START + size; i++)
		c8->ram[i] = data[i - ROM_START];

	return true;
}

/*
 * cycle_cpu
 * ---------
 *
 * Simulates one full CPU cycle. Fetches, decodes, and
 * executes an opcode
 *
 * **c8: pointer of a pointer to a chip8_t struct
 * **instr: pointer of a pointer to a instr_t struct
 */
void cycle_cpu(chip8_t **c8, instr_t **instr){
	/* Retrieve Opcode by combining two 8-bit numbers into one 16-bit number */
	(*instr)->opcode = ((((*c8)->ram[(*c8)->pc]) << 8)) | (((*c8)->ram[(*c8)->pc + 1]) & 0xFF);
	
	/* Decode and execute opcode */
	switch(((*instr)->opcode & 0xF000) >> 12){
		case 0x0:
			switch((*instr)->opcode){
				case 0x00E0: OP_CLS(*c8, *instr); break;
				case 0x00EE: OP_RET(*c8, *instr); break;
			} break;
		case 0x1: OP_JP_ADDR(*c8, *instr); break;
		case 0x2: OP_CALL_ADDR(*c8, *instr); break;
		case 0x3: OP_SE_VX_BYTE(*c8, *instr); break;
		case 0x4: OP_SNE_VX_BYTE(*c8, *instr); break;
		case 0x5: OP_SE_VX_VY(*c8, *instr); break;
		case 0x6: OP_LD_VX_BYTE(*c8, *instr); break;
		case 0x7: OP_ADD_VX_BYTE(*c8, *instr); break;
		case 0x8:
			switch((*instr)->opcode & 0x000F){
				case 0x0: OP_LD_VX_VY(*c8, *instr); break;
				case 0x1: OP_OR_VX_VY(*c8, *instr); break;
				case 0x2: OP_AND_VX_VY(*c8, *instr); break;
				case 0x3: OP_XOR_VX_VY(*c8, *instr); break;
				case 0x4: OP_ADD_VX_VY(*c8, *instr); break;
				case 0x5: OP_SUB_VX_VY(*c8, *instr); break;
				case 0x6: OP_SHR_VX(*c8, *instr); break;
				case 0x7: OP_SUB_VX_VY(*c8, *instr); break;
				case 0xE: OP_SHL_VX(*c8, *instr); break;
				default: fprintf(stderr, "Unknown Opcode\n"); exit(1); break;
			} break;
		case 0x9: OP_SNE_VX_VY(*c8, *instr); break;
		case 0xA: OP_LD_I_ADDR(*c8, *instr); break;
		case 0xB: OP_JP_V0_ADDR(*c8, *instr); break;
		case 0xC: OP_RND_VX_BYTE(*c8, *instr); break;
		case 0xD: OP_DRW_VX_VY_NIB(*c8, *instr); break;
		case 0xE:
			switch((*instr)->opcode & 0x00FF){
				case 0x9E: OP_SKP_VX(*c8, *instr); break;
				case 0xA1: OP_SKNP_VX(*c8, *instr); break;
				default: fprintf(stderr, "Unknown Opcode\n"); exit(1); break;
			} break;
		case 0xF:
			switch((*instr)->opcode & 0x00FF){
				case 0x7: OP_LD_VX_DT(*c8, *instr); break;
				case 0xA: OP_LD_VX_K(*c8, *instr); break;
				case 0x15: OP_LD_DT_VX(*c8, *instr); break;
				case 0x18: OP_LD_ST_VX(*c8, *instr); break;
				case 0x1E: OP_ADD_I_VX(*c8, *instr); break;
				case 0x29: OP_LD_F_VX(*c8, *instr); break;
				case 0x33: OP_LD_B_VX(*c8, *instr); break;
				case 0x55: OP_LD_I_VX(*c8, *instr); break;
				case 0x65: OP_LD_VX_I(*c8, *instr); break;
				default: fprintf(stderr, "Unknown Opcode\n"); exit(1); break;
			} break;
		default:
			fprintf(stderr, "Unknown Opcode\n"); exit(1); break;
	}

	/* Decrement delay timer after delay based events */
	if((*c8)->dt > 0) (*c8)->dt--;
}

/*
 * stack_push
 * ----------
 *
 * Pushes an address onto the CPU stack
 *
 * adr: the address being pushed
 */
void stack_push(chip8_t *c8, u16 adr){
	c8->sp += 0x1;
	c8->stack[c8->sp] = adr;
}

/*
 * stack_pop
 * ---------
 *
 * Pops the address at the top of the CPU stack
 *
 * returns: the address at the top of the stack
 */
u16 stack_pop(chip8_t *c8){
	u16 entry = c8->stack[c8->sp];
	c8->sp -= 0x1;
	return entry;
}
