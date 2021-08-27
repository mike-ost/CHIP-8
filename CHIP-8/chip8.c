#include <stdio.h>
//#include <stdlib.h>
//#include <limits.h>
//#include <float.h>

//#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

//#include "cpu.h"

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 32

int main()
{

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
	unsigned char memory[4095] = {0};

	unsigned char registers[16] = {0};
	unsigned char stack[16] = {0};

	unsigned short i = 0;
	unsigned short pc = 0;

	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;

	unsigned char display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = {0};

	//keep state of keys pressed. 0 = not pressed, 1 = pressed
	unsigned char keys[16] = {0};

	//write fontset to memory
	int x = 0;
	for (x = 0; x < 80; x++)
	{
		memory[x] = fontSet[x];
	}

	//Read rom
	unsigned char buffer[3583] = {0};
	FILE *fptr;
	errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\ibm.ch8", "rb");

	if (err || fptr == NULL)
	{
		printf("FILE ERROR!");
		return 0;
	}
	else
	{
		//fread_s(buffer, sizeof(buffer), 1, sizeof(buffer), fptr);
		fread(buffer, sizeof(buffer), 1, fptr);
	}

	fclose(fptr);

	for (x = 0; x < sizeof(buffer); x++)
	{
		memory[x + 512] = buffer[x];
	}

	//unsigned char byte0 = 0;
	//unsigned char byte1 = 0;

	pc = 0x200;

	while (1)
	{

		//byte0 = memory[pc];		//162
		//byte1 = memory[pc + 1]; //42

		//opCode = byte0;

		opCode = (memory[pc] << 8) | memory[pc + 1]; //41514
													 //opCode = (byte0 << 8); //41514

		pc = pc + 2;

		if (pc > 4094)
		{
			pc = 511;
		}

		//unsigned short temp0 = 0;

		i = opCode & 0x0FFF;
		//temp0 = opCode & 0xF000;

		switch (opCode & 0xF000)
		{
		case 0x0000:;
			//int test1 = 1;

			if (i != 0)
			{
				printf("0");
			}

			break;
		case 0x1000:
			printf("1");
			pc = opCode & 0x0FFF;
			break;
		case 0x2000:
			printf("2");
			break;
		case 0x6000:
			printf("6");
			break;
		case 0x7000:
			printf("7");
			break;
		case 0xA000:
			printf("A");
			break;
		case 0xD000:
			printf("D");
			break;
		default:
			//printf("Not implemented");
			break;
		}

		Sleep(10);
	}

	/* 00e0 a22a 600c 6108 d01f 7009 a239 d01f
a248 7008 d01f 7004 a257 d01f 7008 a266
d01f 7008 a275 d01f 1228 ff00 ff00 3c00
3c00 3c00 3c00 ff00 ffff 00ff 0038 003f
003f 0038 00ff 00ff 8000 e000 e000 8000
8000 e000 e000 80f8 00fc 003e 003f 003b
0039 00f8 00f8 0300 0700 0f00 bf00 fb00
f300 e300 43e0 00e0 0080 0080 0080 0080
00e0 00e0  */

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

	printf("EXIT");

	return 0;
}