/*
 * mem.c - contains functions related to memory manipulation
 */

#include "mem.h"

void load_font(uint8_t *mem){
	int jj = 0;
	for(uint16_t ii = 0x50; ii <= 0x9F; ii += 0x1){
		mem[ii] = FONT_VAL[jj];
		jj++;
	}
}
