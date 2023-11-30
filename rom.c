/*
 * rom.c - contains all info and functions related to inputting CHIP-8 rom
 */

#include "rom.h"
#include <stdint.h>
#include <stdio.h>
#define BUFF 512

typedef struct{
	char file_name[BUFF];
	uint32_t file_size;
	uint8_t *file_data; 
} RomInfo;

static RomInfo curr_rom;

/*
 * opens rom for use, returns true if all for curr_rom are filled with no error, false otherwise
 */
bool open_rom(char *arg){
	FILE *fp;
	uint16_t hex;

	printf("Opening Rom: \"%s\"...\n", arg);
	//sleep(1);

	fp = fopen(arg, "r");
	if(fp == NULL){
		puts("ERROR:\tFile does not exist.");
		return false;
	}

	//stores file name
	strncpy(curr_rom.file_name, arg, BUFF);

	//stores file size
	fseek(fp, 0, SEEK_END);
	curr_rom.file_size = ftell(fp);
	rewind(fp);

	//stores file data
	curr_rom.file_data = malloc(curr_rom.file_size);
	fread(curr_rom.file_data, curr_rom.file_size, 1, fp);
	rewind(fp);

	fclose(fp);

	puts("Success!\n");
	printf("Program Name\t: %s\n", curr_rom.file_name);
	printf("File Size\t: %d Bytes\n", curr_rom.file_size);
	printf("Full File Data\t:\n\n");

	//prints full hex contents of file
	for(int i = 0; i < curr_rom.file_size; i++){
		if(i == 0) printf("%02hx ", curr_rom.file_data[i]);
		else if(i % 20 == 0) printf("%02hx\n", curr_rom.file_data[i]);
		else printf("%02hx ", curr_rom.file_data[i]);
	}

	puts("\n");
	return true;
}
