#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <stdalign.h>

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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef enum{
	RUNNING,
	PAUSED,
	QUIT
} state_t;

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

typedef struct{
	int instr_per_second;
	int scale;
} config_t;
