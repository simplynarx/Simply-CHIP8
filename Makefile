all:
	gcc -o simply-chip8 chip8.c $(shell pkg-config --cflags --libs sdl2)
	gcc -g -fsanitize=address,undefined chip8.c $(shell pkg-config --cflags --libs sdl2)
#	gcc -fsanitize=address chip8.c $(shell pkg-config --cflags --libs sdl2)
