/*
 * display.c - contains methods relating to the SDL window being used to display the graphics of the CHIP-8
 */

#include "display.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

/*
 * Function: initialize_display
 * ----------------------------
 *
 * Initalizes the display for the CHIP-8, creating 
 * a window.
 */
void initialize_display(){
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Simply-CHIP8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

/*
 * Function: update_display
 * ------------------------
 *
 * Updates the display after every CPU cycle.
 * Checks for the window being closed, and 
 * for keypad input
 *
 * *buffer: buffer for raw pixel data
 * display_pitch: the number of bytes in a row of pixel data
 * *keypad: the CHIP-8 keypad
 */
void update_display(void const *buffer, int display_pitch, bool *keypad){
    SDL_Event e;
    while(SDL_PollEvent(&e) > 0){
        switch(e.type){
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_x: keypad[0x0] = 1; break;
                    case SDLK_1: keypad[0x1] = 1; break;
                    case SDLK_2: keypad[0x2] = 1; break;
                    case SDLK_3: keypad[0x3] = 1; break;
                    case SDLK_q: keypad[0x4] = 1; break;
                    case SDLK_w: keypad[0x5] = 1; break;
                    case SDLK_e: keypad[0x6] = 1; break;
                    case SDLK_a: keypad[0x7] = 1; break;
                    case SDLK_s: keypad[0x8] = 1; break;
                    case SDLK_d: keypad[0x9] = 1; break;
                    case SDLK_z: keypad[0xA] = 1; break;
                    case SDLK_c: keypad[0xB] = 1; break;
                    case SDLK_4: keypad[0xC] = 1; break;
                    case SDLK_r: keypad[0xD] = 1; break;
                    case SDLK_f: keypad[0xE] = 1; break;
                    case SDLK_v: keypad[0xF] = 1; break;
                } break;
            case SDL_KEYUP:
                switch(e.key.keysym.sym){
                    case SDLK_x: keypad[0x0] = 0; break;
                    case SDLK_1: keypad[0x1] = 0; break;
                    case SDLK_2: keypad[0x2] = 0; break;
                    case SDLK_3: keypad[0x3] = 0; break;
                    case SDLK_q: keypad[0x4] = 0; break;
                    case SDLK_w: keypad[0x5] = 0; break;
                    case SDLK_e: keypad[0x6] = 0; break;
                    case SDLK_a: keypad[0x7] = 0; break;
                    case SDLK_s: keypad[0x8] = 0; break;
                    case SDLK_d: keypad[0x9] = 0; break;
                    case SDLK_z: keypad[0xA] = 0; break;
                    case SDLK_c: keypad[0xB] = 0; break;
                    case SDLK_4: keypad[0xC] = 0; break;
                    case SDLK_r: keypad[0xD] = 0; break;
                    case SDLK_f: keypad[0xE] = 0; break;
                    case SDLK_v: keypad[0xF] = 0; break;
                } break;
        } break; 
    }

    SDL_UpdateTexture(texture, NULL, buffer, display_pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
