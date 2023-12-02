/*
 * emu.c - main file where all emulator functons are executed
 */

#include "cpu.h"
#include "rom.h"
#include "mem.h"
#include "stack.h"

static ROM_INFO curr_rom;
CPU cpu;
static uint8_t mem[MEM_SIZE];
static uint16_t stack[STACK_SIZE];

/*
 * Function: initalize
 * -------------------
 *
 * Initalizes all emulator functions
 *
 * *arg: the rom file passed in as an argument
 */
void initalize(char *arg){
	load_rom(&curr_rom, mem, arg);
	load_font(mem);
	initialize_cpu(&cpu, stack, mem);
}
