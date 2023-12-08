#pragma once

#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_timer.h>

typedef struct{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} sdl_t;

bool init_sdl(sdl_t *sdl);
bool update_sdl(sdl_t *sdl, chip8_t *c8);
void teardown_sdl(sdl_t *sdl);
