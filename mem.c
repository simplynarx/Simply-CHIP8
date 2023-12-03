/*
 * mem.c - contains functions related to memory manipulation
 */

#include "mem.h"

/*
 * Function: load_font
 * -------------------
 *
 * Loads the font used by certain CHIP-8
 * programs to display hex data into memory
 *
 * *mem: the address bus
 */
void load_font(uint8_t *mem){
	int jj = 0;
	for(uint16_t ii = 0x50; ii <= 0x9F; ii += 0x1){
		mem[ii] = FONT_VAL[jj];
		jj++;
	}

	/*for(uint16_t ii = 0x0; ii <= MEM_SIZE; ii += 0x1){
		printf("%02hx ", mem[ii]);
	}*/
}
