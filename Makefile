all:
	gcc -c main.c rom.c cpu.c display.c emu.c mem.c stack.c
	gcc -o simply-chip8 main.o rom.o cpu.o display.o emu.o mem.o stack.o $(shell pkg-config --cflags --libs sdl2)
