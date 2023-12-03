/*
 * stack.h - contains functions used to manipulate the CPU stack
 */

#include "stack.h"
#include "cpu.h"

/*
 * Function: stack_push
 * --------------------
 *
 * Pushes the current address of the program
 * counter onto the stack.
 *
 * *cpu: the CPU
 * *stack: the CPU stack
 * adr: the current address that the program
 * 			counter is pointing to
 */
void stack_push(CPU *cpu, uint16_t *stack,  uint16_t adr){
	cpu->sp += 0x1;
	stack[cpu->sp] = adr;
}

/*
 * Function: stack_pop
 * -------------------
 *
 * Pops the address at the top of the stack
 *
 * *cpu: the CPU
 * *stack: the stack
 *
 * returns: the entry (address) at the top of
 * 					the stack
 */
uint16_t stack_pop(CPU *cpu, uint16_t *stack){
	uint16_t entry = stack[cpu->sp];
	cpu->sp -= 0x1;
	return entry;
}

