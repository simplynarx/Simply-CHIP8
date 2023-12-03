/*
 * emu.c - main file where all emulator functons are executed
 */

#include "cpu.h"
#include "display.h"
#include "rom.h"
#include "mem.h"
#include "stack.h"

static ROM_INFO curr_rom;
static CPU cpu;
static uint8_t mem[MEM_SIZE];
static uint16_t stack[STACK_SIZE];
static uint32_t video_mem[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static bool keypad[16];

/*
 * Function: initalize
 * -------------------
 *
 * Initalizes all emulator functions and infinitely loops
 * while the program file is open
 *
 * *arg: the rom file passed in as an argument
 * cycle_delay: the desired cpu cycle delay in milliseconds (ms)
 */
void initalize(char *arg, int cycle_delay){
	int display_pitch = sizeof(video_mem[0]) * DISPLAY_WIDTH;

	initialize_display();
	load_font(mem);
	load_rom(&curr_rom, mem, arg);
	cpu.pc = 0x200;
	cpu.sp = 0x1;

	//infinite loop after program is loaded into memory
	do{
	usleep(cycle_delay * 1000);
	if(cpu.delay > 0) cpu.delay--;
	cycle_cpu(&cpu, stack, video_mem, mem, keypad, display_pitch);
	update_display(video_mem, display_pitch, keypad);
	} while(true);
}
