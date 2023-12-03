#pragma once

#include "common.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define SCALE 16

void initialize_display();
void update_display(void const *buffer, int display_pitch, bool *keypad);
