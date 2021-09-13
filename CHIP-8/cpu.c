#include "inc/cpu.h"
#include <stdio.h>
#include <time.h>


struct CPU init_cpu() {
	struct CPU cpu;
	//srand((unsigned int)time(NULL));

	cpu.opCode = 0;
	cpu.sp = 0;
	cpu.index = 0;
	cpu.pc = ADDRESS_ROM_START;
	cpu.delayTimer = 0;
	cpu.soundTimer = 0;
	cpu.drawFlag = 0;

	for (int x = 0; x < REGISTER_SIZE; x++)
	{
		cpu.reg[x] = 0;
	}

	for (int x = 0; x < STACK_SIZE; x++)
	{
		cpu.stack[x] = 0;
	}

    return cpu;
}

/* void testA(byte_t *a, byte_t *b, byte_t *c)
{
	*a = 2;
	*b = 3;
	c += 4;
	*c = 4;
} */

/* void testB()
{
	byte_t a = 1;
	byte_t b = 1;
	byte_t c[5] = {0, 0, 0, 0, 0};

	//testA(&a, &b, c);
} */

void cpu_test(struct CPU *cpu) {
    cpu->pc = ADDRESS_ROM_START;
}

void emulate_cycle(struct CPU *cpu, byte_t *memory, byte_t *display, byte_t *keys) {

}