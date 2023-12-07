all:
	gcc -o simply-chip8 chip8.c $(shell pkg-config --cflags --libs sdl2)
