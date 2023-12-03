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
 * Initalizes all emulator functions
 *
 * *arg: the rom file passed in as an argument
 */
void initalize(char *arg){
	int display_pitch = sizeof(video_mem[0]) * DISPLAY_WIDTH;
	bool window_open = true;
	int cycle_delay = 3;

	initialize_display();
	load_font(mem);
	load_rom(&curr_rom, mem, arg);
	cpu.pc = 0x200;
	cpu.sp = 0x1;

	do{
	usleep(3000);
	cycle_cpu(&cpu, stack, video_mem, mem, keypad, display_pitch);
	update_display(video_mem, display_pitch);
	} while(window_open);
}
