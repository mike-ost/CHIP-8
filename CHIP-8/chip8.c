#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "cpu.h"


#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 32


int main() {

	unsigned char fontSet[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};


	unsigned short opCode = 0;

	//4 kb of memory
	unsigned char memory[4095] = { 0 };

	unsigned char registers[16] = { 0 };
	unsigned char stack[16] = { 0 };


	unsigned short i = 0;
	unsigned short pc = 0;

	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;


	unsigned char display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = { 0 };

	//keep state of keys pressed. 0 = not pressed, 1 = pressed
	unsigned char keys[16] = { 0 };


	//write fontset to memory
	int x = 0;
	for (x = 0; x < 80; x++) {
		memory[x] = fontSet[x];
	}


	//Read rom
	unsigned char buffer[3583] = { 0 };
	FILE* fptr;
	errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\ibm.ch8", "rb");
	
	if (err || fptr == NULL)
	{
		printf("FILE ERROR!");
		return 0;
	}
	else
	{
		fread_s(buffer, sizeof(buffer), 1, sizeof(buffer), fptr);
	}

	fclose(fptr);




	//unsigned char test1 = 0;

	//init_cpu();

	//while (1)
	//{
	//    test1++;
	//    printf("Hello world.\n");

	//    if (test1 > 10) {
	//        break;
	//    }

	//    Sleep(1000);
	//}

	//printf("CHAR_BIT    :   %d\n", CHAR_BIT);
	//printf("CHAR_MAX    :   %d\n", CHAR_MAX);
	//printf("CHAR_MIN    :   %d\n", CHAR_MIN);
	//printf("INT_MAX     :   %d\n", INT_MAX);
	//printf("INT_MIN     :   %d\n", INT_MIN);
	//printf("LONG_MAX    :   %ld\n", (long)LONG_MAX);
	//printf("LONG_MIN    :   %ld\n", (long)LONG_MIN);
	//printf("SCHAR_MAX   :   %d\n", SCHAR_MAX);
	//printf("SCHAR_MIN   :   %d\n", SCHAR_MIN);
	//printf("SHRT_MAX    :   %d\n", SHRT_MAX);
	//printf("SHRT_MIN    :   %d\n", SHRT_MIN);
	//printf("UCHAR_MAX   :   %d\n", UCHAR_MAX);
	//printf("UINT_MAX    :   %u\n", (unsigned int)UINT_MAX);
	//printf("ULONG_MAX   :   %lu\n", (unsigned long)ULONG_MAX);
	//printf("USHRT_MAX   :   %d\n", (unsigned short)USHRT_MAX);

	return 0;
}