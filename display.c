/*************************
 * SDL RELATED FUNCTIONS *
 *************************/

#include "display.h"

/*
 * init_sdl
 * --------
 *
 * Initializes SDL and its subsystems
 *
 * *sdl: an sdl_t struct
 *
 * returns: false if SDL error, true otherwise
 */
bool init_sdl(sdl_t *sdl){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		SDL_Log("ERROR: Could not initalize SDL. %s\n", SDL_GetError());
		return false;
	}
	
	sdl->window = SDL_CreateWindow("Simply-CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	if(!sdl->window){
		SDL_Log("ERROR: Could not create SDL Window. %s\n", SDL_GetError());
		return false;
	}

	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if(!sdl->renderer){
		SDL_Log("ERROR: Could not create SDL Renderer. %s\n", SDL_GetError());
		return false;
	}

	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if(!sdl->texture){
		SDL_Log("ERROR: Could not create SDL Texture. %s\n", SDL_GetError());
		return false;
	}

	return true;
}

/*
 * update_sdl
 * ----------
 *
 * Updates the SDL Texture and Renderer with the current state
 * of the CHIP8's VRAM. Checks for user SDL_KEY input
 *
 * *sdl: an sdl_t struct
 */
bool update_sdl(sdl_t *sdl, chip8_t *c8){
	SDL_Event e;
	while(SDL_PollEvent(&e) > 0){
		switch(e.type){
			/* If the window is closed */
			case SDL_QUIT:
				c8->state = QUIT;
				break;
			/* When a key is pressed / held down */
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym){
          case SDLK_x: c8->keypad[0x0] = 1; break;
          case SDLK_1: c8->keypad[0x1] = 1; break;
          case SDLK_2: c8->keypad[0x2] = 1; break;
          case SDLK_3: c8->keypad[0x3] = 1; break;
          case SDLK_q: c8->keypad[0x4] = 1; break;
          case SDLK_w: c8->keypad[0x5] = 1; break;
          case SDLK_e: c8->keypad[0x6] = 1; break;
          case SDLK_a: c8->keypad[0x7] = 1; break;
          case SDLK_s: c8->keypad[0x8] = 1; break;
          case SDLK_d: c8->keypad[0x9] = 1; break;
          case SDLK_z: c8->keypad[0xA] = 1; break;
          case SDLK_c: c8->keypad[0xB] = 1; break;
          case SDLK_4: c8->keypad[0xC] = 1; break;
          case SDLK_r: c8->keypad[0xD] = 1; break;
          case SDLK_f: c8->keypad[0xE] = 1; break;
          case SDLK_v: c8->keypad[0xF] = 1; break;
      } break;
			/* When a key is released */
      case SDL_KEYUP:
        switch(e.key.keysym.sym){
          case SDLK_x: c8->keypad[0x0] = 0; break;
          case SDLK_1: c8->keypad[0x1] = 0; break;
          case SDLK_2: c8->keypad[0x2] = 0; break;
          case SDLK_3: c8->keypad[0x3] = 0; break;
          case SDLK_q: c8->keypad[0x4] = 0; break;
          case SDLK_w: c8->keypad[0x5] = 0; break;
          case SDLK_e: c8->keypad[0x6] = 0; break;
          case SDLK_a: c8->keypad[0x7] = 0; break;
          case SDLK_s: c8->keypad[0x8] = 0; break;
          case SDLK_d: c8->keypad[0x9] = 0; break;
          case SDLK_z: c8->keypad[0xA] = 0; break;
          case SDLK_c: c8->keypad[0xB] = 0; break;
          case SDLK_4: c8->keypad[0xC] = 0; break;
          case SDLK_r: c8->keypad[0xD] = 0; break;
          case SDLK_f: c8->keypad[0xE] = 0; break;
          case SDLK_v: c8->keypad[0xF] = 0; break;
      } break;

		}
	}

	/* Update display with the vram buffer */
	SDL_UpdateTexture(sdl->texture, NULL, c8->vram, sizeof(c8->vram[0]) * DISPLAY_WIDTH);
	SDL_RenderClear(sdl->renderer);
	SDL_RenderCopy(sdl->renderer, sdl->texture, NULL, NULL);
	SDL_RenderPresent(sdl->renderer);

	return true;
}

/*
 * teardown_sdl
 * ------------
 *
 * Tearsdown SDL and all of its subsystems
 *
 * *sdl: an sdl_t struct
 */
void teardown_sdl(sdl_t *sdl){
	SDL_DestroyRenderer(sdl->renderer);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}
