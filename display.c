#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
//static SDL_Surface *screen;

void initialize_display(){
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void update_display(void const *buffer, int display_pitch){
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0){
        switch(e.type){
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            default:
                SDL_UpdateTexture(texture, NULL, buffer, display_pitch);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
                break;
        } break; 
    }
}
