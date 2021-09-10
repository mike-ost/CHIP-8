#include "inc/cpu.h"


struct CPU init_cpu() {
	struct CPU cpu;

	cpu.opCode = 0;
	cpu.sp = 0;
	cpu.i = 0;
	cpu.pc = 0;
	cpu.delayTimer = 0;
	cpu.soundTimer = 0;
	cpu.drawFlag = 0;

	for (int x = 0; x < sizeof(cpu.registers); x++)
	{
		cpu.registers[x] = 0;
	}

	for (int x = 0; x < sizeof(cpu.stack); x++)
	{
		cpu.stack[x] = 0;
	}

    return cpu;
}


void cpu_test(struct CPU *cpu) {
    cpu->pc = 0x200;
}

void emulate_cycle(struct CPU *cpu, unsigned char *memory, unsigned char *display, unsigned char *keys) {
    
}