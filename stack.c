#include "stack.h"
#include "cpu.h"

void stack_push(CPU *cpu, uint16_t *stack,  uint16_t adr){
	cpu->sp += 0x1;
	stack[cpu->sp] = adr;
	//cpu->sp++;
}

uint16_t stack_pop(CPU *cpu, uint16_t *stack){
	uint16_t entry = stack[cpu->sp];
	//stack[cpu->sp] = 0x0;
	cpu->sp -= 0x1;
	return entry;
}

