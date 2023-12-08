#include "chip8.h"
#include "display.h"
#include "instructions.h"

void set_config(config_t *config, int argc, char **argv);

//Main
int main(int argc, char **argv){
	chip8_t c8 = {0};
	sdl_t sdl = {0};
	instr_t instr = {0};
	config_t config = {0};

	chip8_t *c8_ptr = &c8;
	instr_t *instr_ptr = &instr;

	u64 start_frame, end_frame;
	double time_elapsed;

	/* If no ROM file is passed in as an argument */
	if(argc < 2){
		fprintf(stderr, "ERROR: Too few arguments.\n");
		exit(EXIT_FAILURE);
	}

	/* Sets any config passed in as argument */
	set_config(&config, argc, argv);

	/* Set seed for opcode OP_RND_VX_BYTE */
	srand(time(NULL));

	/* Initalize SDL */
	if(!init_sdl(&sdl)) exit(EXIT_FAILURE);

	/* Initalize CHIP-8 system */
	init_chip8(&c8);

	/* Load rom */
	if(!load_rom(&c8, argv[1])) exit(EXIT_FAILURE);

	/* Main emulator loop */
	while(c8.state != QUIT){
		start_frame = SDL_GetPerformanceCounter();

		/* Run program at 60hz */
		for(u32 i = 0; i < config.instr_per_second / 60; i++)
			/* Complete one CPU cycle */
			cycle_cpu(&c8_ptr, &instr_ptr);

		end_frame = SDL_GetPerformanceCounter();

		time_elapsed = (double)((end_frame - start_frame) / 1000) / SDL_GetPerformanceFrequency();

		SDL_Delay(16.67f > time_elapsed ? 16.67f - time_elapsed : 0);

		/* Update display */
		if(!update_sdl(&sdl, &c8)) exit(EXIT_FAILURE);
	}

	/* Ends all SDL processes */
	teardown_sdl(&sdl);

	return 0;
}

/*
 * set_config
 * ----------
 *
 * Sets config based on command line arguments
 *
 * *config: a config_t struct
 * argc: number of arguments
 * **argv: command line arguments
 */
void set_config(config_t *config, int argc, char **argv){
	/* Set defaults */
	config->instr_per_second = 200;

	/* Checks for command line arguments */
	for(int i = 1; i < argc; i++){
		/* Set instructions per second */
		if(strcmp(argv[i], "-i") == 0){
			config->instr_per_second = atoi(argv[i+1]);
			if(atoi(argv[i+1]) < 60) config->instr_per_second = 200;
		} 
	}
}
