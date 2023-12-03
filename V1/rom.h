#pragma once

#include "common.h"
#include <stdint.h>
#define BUFF 512

typedef struct{
	char file_name[BUFF];
	uint32_t file_size;
	uint8_t *file_data; 
} ROM_INFO;

bool load_rom(ROM_INFO *curr_rom, uint8_t *mem, char *arg);
