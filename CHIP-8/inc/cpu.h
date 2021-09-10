#include "constants.h"

#ifndef CPU_H
#define CPU_H

struct CPU init_cpu();
void cpu_test(struct CPU *cpu);
void emulate_cycle(struct CPU *cpu, unsigned char *memory, unsigned char *display, unsigned char *keys);

#endif
