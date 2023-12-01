#include "cpu.h"
#include "rom.h"
#include "mem.h"

static ROM_INFO curr_rom;
static CPU cpu;

void initalize(char *arg){
	open_rom(&curr_rom, mem, arg);
	load_font(mem);
	initialize_cpu(&cpu);
}
