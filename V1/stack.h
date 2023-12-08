#pragma once

#include "common.h"
#include "cpu.h"

#define STACK_SIZE 10

void stack_push(CPU *cpu, uint16_t *stack,  uint16_t adr);
uint16_t stack_pop(CPU *cpu, uint16_t *stack);
