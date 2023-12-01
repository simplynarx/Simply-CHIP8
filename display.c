#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

void initialize_display(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Error: Could not initialize SDL2 library\n%s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
																				SDL_WINDOWPOS_CENTERED, 64, 32, 0);
	if(!window){
		printf("Error: Failed to create window\n%s\n", SDL_GetError());
		exit(1);
	}

	SDL_Surface *window_surface = SDL_GetWindowSurface(window);

	if(!window_surface){
		printf("Error: Failed to get surface from window\n%s\n", SDL_GetError());
		exit(1);
	}

	//SDL_Renderer *renderer = SDL_GetRenderer(window);
	//SDL_RenderSetLogicalSize(renderer, 512, 256);

	bool keep_window_open = true;
	while(keep_window_open){
		SDL_Event e;

		while(SDL_PollEvent(&e) > 0){

			switch(e.type){
				case SDL_QUIT:
					keep_window_open = false;
					break;
			}

			SDL_UpdateWindowSurface(window);
		}
	}
}
