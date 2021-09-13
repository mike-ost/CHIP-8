#include "constants.h"

#ifndef CPU_H
#define CPU_H

struct CPU init_cpu();
void cpu_test(struct CPU *cpu);
void emulate_cycle(struct CPU *cpu, unsigned char *memory, unsigned char *display, unsigned char *keys);

struct CPU
{
	num_t opCode;
	byte_t reg[REGISTER_SIZE];
	num_t stack[STACK_SIZE];
	num_t sp;
	num_t index;
	num_t pc;
	unsigned char delayTimer;
	unsigned char soundTimer;
	unsigned char drawFlag;
};

#endif
