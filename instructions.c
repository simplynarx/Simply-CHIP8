/********************
 * CPU Instructions *
 ********************/

#include "instructions.h"
#include "chip8.h"

/**
 *
 * 			Key
 * -------------
 *
 * VX   : Register #X
 * VY   : Register #Y
 * VF 	: Flag register, only contains 0 or 1
 * KK   : A byte value
 * NNN  : A memory address
 * N 		: The last nibble of a 16 bit hex number
 *
 * For more info on specific opcodes: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0
 */

//00E0 - clear the display
void OP_CLS(chip8_t *c8, instr_t *instr){
	memset(c8->vram, 0, sizeof(c8->vram));
	c8->pc += 0x2;
}

//00EE - return from a subroutine
void OP_RET(chip8_t *c8, instr_t *instr){
	c8->pc = stack_pop(c8);
	c8->pc += 0x2;
}

//1NNN - jump to address NNN
void OP_JP_ADDR(chip8_t *c8, instr_t *instr){
	c8->pc = CURR_OP_NNN;
}

//2NNN - call subroutine at address NNN
void OP_CALL_ADDR(chip8_t *c8, instr_t *instr){
	stack_push(c8, c8->pc);
	c8->pc = CURR_OP_NNN;
}

//3XKK - skip next instruction if VX = KK
void OP_SE_VX_BYTE(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VX] == CURR_OP_KK) c8->pc += 0x4;
	else c8->pc += 0x2;
}

//4XKK - skip next instruction if VX = KK
void OP_SNE_VX_BYTE(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VX] != CURR_OP_KK) c8->pc += 0x4;
	else c8->pc += 0x2;
}

//5XY0 - skip next instruction if VX = VY
void OP_SE_VX_VY(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VX] == c8->reg[CURR_OP_VY]) c8->pc += 0x4;
	else c8->pc += 0x2;
}

//6XKK - store KK in VX
void OP_LD_VX_BYTE(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] = CURR_OP_KK;
	c8->pc += 0x2;
}

//7XKK - add KK to VX
void OP_ADD_VX_BYTE(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] += CURR_OP_KK;
	c8->pc += 0x2;
}

//8XY0 - set VX = VY
void OP_LD_VX_VY(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] = c8->reg[CURR_OP_VY];
	c8->pc += 0x2;
}

//8XY1 - sets VX to VX | VY (bitwise OR)
void OP_OR_VX_VY(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] |= c8->reg[CURR_OP_VY];
	c8->pc += 0x2;
}

//8XY2 - sets VX to VX & VY (bitwise AND)
void OP_AND_VX_VY(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] &= c8->reg[CURR_OP_VY];
	c8->pc += 0x2;
}

//8XY3 - sets VX to VX ^ VY (bitwise XOR)
void OP_XOR_VX_VY(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] ^= c8->reg[CURR_OP_VY];
	c8->pc += 0x2;
}

//8XY4 - sets VX to VX + VY, set VF to 1 if carry occurs
void OP_ADD_VX_VY(chip8_t *c8, instr_t *instr){
	u16 sum = c8->reg[CURR_OP_VX] + c8->reg[CURR_OP_VY];
	if(sum > 0xFF) c8->reg[0xF] = 0x1;
	else c8->reg[0xF] = 0x0;

	c8->reg[CURR_OP_VX] = sum & 0xFF;
	c8->pc += 0x2;
}

//8XY5 - sets VX to VX - VY, set VF to 1 if borrow does not occur
void OP_SUB_VX_VY(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VX] > c8->reg[CURR_OP_VY]) c8->reg[0xF] = 0x1;
	else c8->reg[0xF] = 0x0;

	c8->reg[CURR_OP_VX] -= c8->reg[CURR_OP_VY];
	c8->pc += 0x2;
}

//8XY6 - sets VX to VX >> 1 (right shift of 1 / division by 2), and
// 			 the least significant bit is stored in VF
void OP_SHR_VX(chip8_t *c8, instr_t *instr){
	c8->reg[0xF] = (c8->reg[0xF] & 0x1);
	c8->reg[CURR_OP_VX] >>= 0x1;
	c8->pc += 0x2;
}

//8XY7 sets VX to VY - VX, set VF to 1 if borrow does not occur
void OP_SUBN_VX_VY(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VY] > c8->reg[CURR_OP_VX]) c8->reg[0xF] = 0x1;
	else c8->reg[0xF] = 0x0;

	c8->reg[CURR_OP_VX] = c8->reg[CURR_OP_VY] - c8->reg[CURR_OP_VX];
	c8->pc += 0x2;
}

//8XYE - sets VX to VX << 1 (left shift of 1 / multiply by 2), and
// 			 the least significant bit is stored in VF
void OP_SHL_VX(chip8_t *c8, instr_t *instr){
	c8->reg[0xF] = (c8->reg[CURR_OP_VX] & 0x8) >> 0x7;
	c8->reg[CURR_OP_VX] <<= 0x1;
	c8->pc += 0x2;
}

//9XY0 - skips next instruction if VX != VY
void OP_SNE_VX_VY(chip8_t *c8, instr_t *instr){
	if(c8->reg[CURR_OP_VX] != c8->reg[CURR_OP_VY]) c8->pc += 0x4;
	else c8->pc += 0x2;
}


//ANNN - set index register to NNN
void OP_LD_I_ADDR(chip8_t *c8, instr_t *instr){
	c8->ir = CURR_OP_NNN;
	c8->pc += 0x2;
}

//BNNN - jump to address NNN + V0
void OP_JP_V0_ADDR(chip8_t *c8, instr_t *instr){
	c8->pc = CURR_OP_NNN + c8->reg[0x0];
}

//CXKK - set VX to (random byte & KK)
void OP_RND_VX_BYTE(chip8_t *c8, instr_t *instr){
	u8 rand_byte = rand() % 0xFF;
	c8->reg[CURR_OP_VX] = (rand_byte & CURR_OP_KK);
	c8->pc += 0x2;
}

//DXYN - display N-byte sprite starting at memory location I
// 			 at (VX, VY), set VF = collision
//Credit: https://austinmorlan.com/posts/chip8_emulator
void OP_DRW_VX_VY_NIB(chip8_t *c8, instr_t *instr){
	u8 height, x_pos, y_pos, sprite_byte, sprite_pixel;
	u32 *screen_pixel;

	height = CURR_OP_N;
	x_pos = c8->reg[CURR_OP_VX] % DISPLAY_WIDTH;
	y_pos= c8->reg[CURR_OP_VY] % DISPLAY_HEIGHT;
	c8->reg[0xF] = 0;

	for(u32 row = 0; row < height; ++row){
		sprite_byte = c8->ram[c8->ir + row];

		for(u32 col = 0; col < 8; ++col){
			sprite_pixel = sprite_byte & (0x80 >> col);
			screen_pixel = &c8->vram[(y_pos + row) * DISPLAY_WIDTH + (x_pos + col)];

			if(sprite_pixel){
				if(*screen_pixel == 0xFFFFFFFF){
					c8->reg[0xF] = 1;
				}

				*screen_pixel ^= 0xFFFFFFFF;
			}
		}
	}

	c8->pc += 0x2;
}

//EX9E - skip next instruction if key with value of VX is pressed
void OP_SKP_VX(chip8_t *c8, instr_t *instr){
	if(c8->keypad[c8->reg[CURR_OP_VX]]) c8->pc += 0x4;
	else c8->pc += 0x2;
}

//EXA1 - skip next instruction if key with value of VX is not pressed
void OP_SKNP_VX(chip8_t *c8, instr_t *instr){
	if(!c8->keypad[c8->reg[CURR_OP_VX]]) c8->pc += 0x4;
	else c8->pc += 0x2;
}

//FX07 - set VX to delay timer value
void OP_LD_VX_DT(chip8_t *c8, instr_t *instr){
	c8->reg[CURR_OP_VX] = c8->dt;
	c8->pc += 0x2;
}

//FX0A - wait for a key press, and store the value of the key in VX
void OP_LD_VX_K(chip8_t *c8, instr_t *instr){
	for(u8 i = 0; i < NUM_KEYS; i++){
		if(c8->keypad[i]){
			c8->reg[0xF] = i;
			break;
		} 
		else c8->pc -= 0x2;
	}
	c8->pc += 0x2;
}

//FX15 - set delay timer to VX
void OP_LD_DT_VX(chip8_t *c8, instr_t *instr){
	c8->dt = c8->reg[CURR_OP_VX];
	c8->pc += 0x2;
}

//FX18 - set sound timer to VX
void OP_LD_ST_VX(chip8_t *c8, instr_t *instr){
	c8->st = c8->reg[CURR_OP_VX];
	c8->pc += 0x2;
}

//FX1E - set index c8->register to index c8->reg[ster + VX
void OP_ADD_I_VX(chip8_t *c8, instr_t *instr){
	c8->ir += c8->reg[CURR_OP_VX];
	c8->pc += 0x2;
}

//FX29 - set index c8->register to location of sprite for digit VX (font)
void OP_LD_F_VX(chip8_t *c8, instr_t *instr){
	c8->ir = FONT_START + (5 * c8->reg[CURR_OP_VX]);
	c8->pc += 0x2;
}

//FX33 - store binary-coded-decimal representation of VX at memory locations
// 			 index c8->register, index c8->reg[ster + 1, and index register + 2
void OP_LD_B_VX(chip8_t *c8, instr_t *instr){
	u8 nib = c8->reg[CURR_OP_VX];

	c8->ram[c8->ir + 0x2] = nib % 10; nib /= 10;
	c8->ram[c8->ir + 0x1] = nib % 10; nib /= 10;
	c8->ram[c8->ir] = nib % 10;

	c8->pc += 0x2;
}

//FX55 - store c8->registers V0 to VX inclusive starting at index c8->reg[ster
void OP_LD_I_VX(chip8_t *c8, instr_t *instr){
	for(u8 i = 0; i <= CURR_OP_VX; ++i)
		c8->ram[c8->ir + i] = c8->reg[i];
	
	c8->pc += 0x2;
}

//FX65 - read c8->registers V0 to VX inclusive starting at index c8->reg[ster
void OP_LD_VX_I(chip8_t *c8, instr_t *instr){
	for(u8 i = 0; i <= CURR_OP_VX; ++i)
		c8->reg[i] = c8->ram[c8->ir + i];

	c8->pc += 0x2;
}
