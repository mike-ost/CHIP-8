#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define ADDRESS_ROM_START 0x200
#define RAM_SIZE 0x1000
#define REGISTER_SIZE 0x10
#define STACK_SIZE 0x10
#define NUMBER_OF_KEYS 0x10
//#define RAND_MAX 0xFF

struct CPU
{
	unsigned short opCode;
	unsigned char registers[REGISTER_SIZE];
	unsigned char stack[STACK_SIZE];
	unsigned short sp;
	unsigned short i;
	unsigned short pc;
	unsigned char delayTimer;
	unsigned char soundTimer;
	unsigned char drawFlag;
};

#endif
