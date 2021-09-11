#include "inc/cpu.h"
#include <stdio.h>
#include <time.h>


struct CPU init_cpu() {
	struct CPU cpu;
	//srand((unsigned int)time(NULL));

	cpu.opCode = 0;
	cpu.sp = 0;
	cpu.i = 0;
	cpu.pc = ADDRESS_ROM_START;
	cpu.delayTimer = 0;
	cpu.soundTimer = 0;
	cpu.drawFlag = 0;

	for (int x = 0; x < REGISTER_SIZE; x++)
	{
		cpu.registers[x] = 0;
	}

	for (int x = 0; x < STACK_SIZE; x++)
	{
		cpu.stack[x] = 0;
	}

    return cpu;
}


void cpu_test(struct CPU *cpu) {
    cpu->pc = ADDRESS_ROM_START;
}

void emulate_cycle(struct CPU *cpu, unsigned char *memory, unsigned char *display, unsigned char *keys) {

}