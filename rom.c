/*
 * rom.c - contains all info and functions related to inputting CHIP-8 rom
 */

#include "rom.h"
#include "mem.h"

/*
 * Function: 	open_rom
 * -------------------
 *
 * Opens a CHIP-8 rom file and loads it into memory.
 * 
 * *curr_rom: a pointer to the current rom struct
 * *mem: 			a pointer to the memory bus
 * *arg: 			the rom file passed in as an argument
 *
 * returns: false if failed to open rom file, true otherwise
 */
bool open_rom(ROM_INFO *curr_rom, uint8_t *mem, char *arg){
	FILE *fp;

	printf("Opening Rom: \"%s\"...\n", arg);

	fp = fopen(arg, "r");
	if(fp == NULL){
		printf("ERROR:\tFile does not exist\n");
		return false;
	}

	//stores file name
	strncpy(curr_rom->file_name, arg, BUFF); 

	//stores file size
	fseek(fp, 0, SEEK_END);
	curr_rom->file_size = ftell(fp);
	rewind(fp);

	//stores file data
	curr_rom->file_data = malloc(curr_rom->file_size);
	fread(curr_rom->file_data, curr_rom->file_size, 1, fp);
	rewind(fp);

	fclose(fp);

	printf("Success!\n\nProgram Name\t: %s\nFile Size\t: %d Bytes\n\n", 
			curr_rom->file_name, curr_rom->file_size);

	//prints full hex contents of file
	/*for(int i = 0; i < curr_rom->file_size; i++){
		if(i == 0) printf("%02hx ", curr_rom->file_data[i]);
		else if(i % 20 == 0) printf("%02hx\n", curr_rom->file_data[i]);
		else printf("%02hx ", curr_rom->file_data[i]);
	}*/

	//loads rom into memory, starting at address 0x200
	int jj = 0x0;
	for(uint16_t ii = 0x200; ii < 0x200 + curr_rom->file_size; ii += 0x1){
		mem[ii] = curr_rom->file_data[jj];
		jj += 0x1;
	}

	return true;
}
