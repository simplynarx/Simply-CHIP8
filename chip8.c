#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <stdalign.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_pixels.h>

#define RAM_SIZE 4096
#define VRAM_SIZE 2048
#define STACK_SIZE 16
#define NUM_KEYS 16
#define NUM_REGISTERS 16
#define NUM_INST 34
#define FONT_START 0x50
#define FONT_END 0x9F
#define ROM_START 0x200

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define SCALE 16

#define CURR_OP_TYPE ((instr->opcode & 0xF000) >> 12)
#define CURR_OP_VX ((instr->opcode & 0x0F00) >> 8)
#define CURR_OP_VY ((instr->opcode & 0x00F0) >> 4)
#define CURR_OP_NNN (instr->opcode & 0x0FFF)
#define CURR_OP_KK (instr->opcode & 0x00FF)
#define CURR_OP_N (instr->opcode & 0x000F)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef enum{
	QUIT,
	RUNNING,
	PAUSED
} state_t;

typedef struct{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} sdl_t;

typedef struct{
	state_t state;
	u8 reg[NUM_REGISTERS]; //CPU registers
	u8 dt; //Delay timer
	u8 st; //Sound timer
	u16 ir; //Index register
	u16 pc; //Program counter
	u16 sp; //Stack pointer
	u8 ram[RAM_SIZE]; //Memory
	u16 stack[STACK_SIZE]; //CPU stack
	bool keypad[NUM_KEYS]; //Keypad
	u32 vram[DISPLAY_WIDTH * DISPLAY_HEIGHT]; //Video buffer
} chip8_t;

typedef struct{
	u16 opcode;
	char *name;
} instr_t;

static const u8 FONT_VAL[] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

bool init_sdl(sdl_t *sdl);
bool update_sdl(sdl_t *sdl, chip8_t *c8);
void teardown_sdl(sdl_t *sdl);
void init_chip8(chip8_t *c8);
void init_instr_table(void);
bool load_rom(chip8_t *c8, char *rom);
void cycle_cpu(chip8_t **c8, instr_t **instr);

void OP_CLS(chip8_t *c8, instr_t *instr);
void OP_RET(chip8_t *c8, instr_t *instr);
void OP_JP_ADDR(chip8_t *c8, instr_t *instr);
void OP_CALL_ADDR(chip8_t *c8, instr_t *instr);
void OP_SE_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_SNE_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_SE_VX_VY(chip8_t *c8, instr_t *instr);
void OP_LD_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_ADD_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_LD_VX_VY(chip8_t *c8, instr_t *instr);
void OP_OR_VX_VY(chip8_t *c8, instr_t *instr);
void OP_AND_VX_VY(chip8_t *c8, instr_t *instr);
void OP_XOR_VX_VY(chip8_t *c8, instr_t *instr);
void OP_ADD_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SUB_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SHR_VX(chip8_t *c8, instr_t *instr);
void OP_SUBN_VX_VY(chip8_t *c8, instr_t *instr);
void OP_SHL_VX(chip8_t *c8, instr_t *instr);
void OP_SNE_VX_VY(chip8_t *c8, instr_t *instr);
void OP_LD_I_ADDR(chip8_t *c8, instr_t *instr);
void OP_JP_V0_ADDR(chip8_t *c8, instr_t *instr);
void OP_RND_VX_BYTE(chip8_t *c8, instr_t *instr);
void OP_DRW_VX_VY_NIB(chip8_t *c8, instr_t *instr);
void OP_SKP_VX(chip8_t *c8, instr_t *instr);
void OP_SKNP_VX(chip8_t *c8, instr_t *instr);
void OP_LD_VX_DT(chip8_t *c8, instr_t *instr);
void OP_LD_VX_K(chip8_t *c8, instr_t *instr);
void OP_LD_DT_VX(chip8_t *c8, instr_t *instr);
void OP_LD_ST_VX(chip8_t *c8, instr_t *instr);
void OP_ADD_I_VX(chip8_t *c8, instr_t *instr);
void OP_LD_F_VX(chip8_t *c8, instr_t *instr);
void OP_LD_B_VX(chip8_t *c8, instr_t *instr);
void OP_LD_I_VX(chip8_t *c8, instr_t *instr);
void OP_LD_VX_I(chip8_t *c8, instr_t *instr);



/*************************
 * SDL RELATED FUNCTIONS *
 *************************/

/*
 * init_sdl
 * --------
 *
 * Initializes SDL and its subsystems
 *
 * *sdl: an sdl_t struct
 *
 * returns: false if SDL error, true otherwise
 */
bool init_sdl(sdl_t *sdl){
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
		SDL_Log("ERROR: Could not initalize SDL. %s\n", SDL_GetError());
		return false;
	}
	
	sdl->window = SDL_CreateWindow("Simply-CHIP8", SDL_WINDOWPOS_CENTERED, 
																 SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALE,
																 DISPLAY_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	if(!sdl->window){
		SDL_Log("ERROR: Could not create SDL Window. %s\n", SDL_GetError());
		return false;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if(!sdl->renderer){
		SDL_Log("ERROR: Could not create SDL Renderer. %s\n", SDL_GetError());
		return false;
	}

	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888,
																	 SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH,
																	 DISPLAY_HEIGHT);
	if(!sdl->texture){
		SDL_Log("ERROR: Could not create SDL Texture. %s\n", SDL_GetError());
		return false;
	}

	return true;
}

/*
 * update_sdl
 * ----------
 *
 * Updates the SDL Texture and Renderer with the current state
 * of the CHIP8's VRAM. Checks for user SDL_KEY input
 *
 * *sdl: an sdl_t struct
 */
bool update_sdl(sdl_t *sdl, chip8_t *c8){
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0){
		switch(e.type){
			case SDL_QUIT:
				c8->state = QUIT;
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym){
          case SDLK_x: c8->keypad[0x0] = 1; break;
          case SDLK_1: c8->keypad[0x1] = 1; break;
          case SDLK_2: c8->keypad[0x2] = 1; break;
          case SDLK_3: c8->keypad[0x3] = 1; break;
          case SDLK_q: c8->keypad[0x4] = 1; break;
          case SDLK_w: c8->keypad[0x5] = 1; break;
          case SDLK_e: c8->keypad[0x6] = 1; break;
          case SDLK_a: c8->keypad[0x7] = 1; break;
          case SDLK_s: c8->keypad[0x8] = 1; break;
          case SDLK_d: c8->keypad[0x9] = 1; break;
          case SDLK_z: c8->keypad[0xA] = 1; break;
          case SDLK_c: c8->keypad[0xB] = 1; break;
          case SDLK_4: c8->keypad[0xC] = 1; break;
          case SDLK_r: c8->keypad[0xD] = 1; break;
          case SDLK_f: c8->keypad[0xE] = 1; break;
          case SDLK_v: c8->keypad[0xF] = 1; break;
      } break;
      case SDL_KEYUP:
        switch(e.key.keysym.sym){
          case SDLK_x: c8->keypad[0x0] = 0; break;
          case SDLK_1: c8->keypad[0x1] = 0; break;
          case SDLK_2: c8->keypad[0x2] = 0; break;
          case SDLK_3: c8->keypad[0x3] = 0; break;
          case SDLK_q: c8->keypad[0x4] = 0; break;
          case SDLK_w: c8->keypad[0x5] = 0; break;
          case SDLK_e: c8->keypad[0x6] = 0; break;
          case SDLK_a: c8->keypad[0x7] = 0; break;
          case SDLK_s: c8->keypad[0x8] = 0; break;
          case SDLK_d: c8->keypad[0x9] = 0; break;
          case SDLK_z: c8->keypad[0xA] = 0; break;
          case SDLK_c: c8->keypad[0xB] = 0; break;
          case SDLK_4: c8->keypad[0xC] = 0; break;
          case SDLK_r: c8->keypad[0xD] = 0; break;
          case SDLK_f: c8->keypad[0xE] = 0; break;
          case SDLK_v: c8->keypad[0xF] = 0; break;
      } break;

		}
	}

	SDL_UpdateTexture(sdl->texture, NULL, c8->vram, sizeof(c8->vram[0]) * DISPLAY_WIDTH);
	SDL_RenderClear(sdl->renderer);
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	SDL_RenderPresent(sdl->renderer);

	return true;
}

/*
 * teardown_sdl
 * ------------
 *
 * Tearsdown SDL and all of its subsystems
 *
 * *sdl: an sdl_t struct
 */
void teardown_sdl(sdl_t *sdl){
	SDL_DestroyRenderer(sdl->renderer);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}



/***************************
 * CHIP8 RELATED FUNCTIONS *
 ***************************/

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
	//c8->sp = 0x1;

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

	//Possible implementation of file type checking

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

void cycle_cpu(chip8_t **c8, instr_t **instr){
	(*instr)->opcode = ((((*c8)->ram[(*c8)->pc]) << 8)) | (((*c8)->ram[(*c8)->pc + 1]) & 0xFF);
	
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



/********************
 * CPU Instructions *
 ********************/

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


//Main
int main(int argc, char **argv){
	chip8_t c8 = {0};
	sdl_t sdl = {0};
	instr_t instr = {0};

	chip8_t *c8_ptr = &c8;
	instr_t *instr_ptr = &instr;

	if(argc < 3){
		fprintf(stderr, "ERROR: Too few arguments.\n");
		exit(EXIT_FAILURE);
	}

	if(!init_sdl(&sdl)) exit(EXIT_FAILURE);

	init_chip8(&c8);

	if(!load_rom(&c8, argv[1])) exit(EXIT_FAILURE);

	while(c8.state == RUNNING){
		usleep(atoi(argv[2]) * 1000);
		if(c8.dt > 0) c8.dt--; //MOVE TO CYCLE (after opcode is executed)
		cycle_cpu(&c8_ptr, &instr_ptr);
		if(!update_sdl(&sdl, &c8)) exit(EXIT_FAILURE);
	}

	return 0;
}
