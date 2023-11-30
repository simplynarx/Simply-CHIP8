all:
	gcc -c main.c rom.c cpu.c
	gcc -o simply-chip8 main.o rom.o cpu.o
