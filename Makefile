all:
	gcc -c main.c chip8.c display.c instructions.c
	gcc -o simply-chip8 main.o chip8.o display.o instructions.o $(shell pkg-config --cflags --libs sdl2)
