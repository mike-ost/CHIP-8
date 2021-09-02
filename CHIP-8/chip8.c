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

#include "inc/chip8.h"
#include "inc/cpu.h"
#include "inc/display.h"
#include "inc/font.h"
#include "inc/loadrom.h"

//#define ADDRESS_ROM_START 0x200

/* void reset_display(unsigned char *display, int size) {

	for (int x = 0; x < size; x++) {
		*display = 0;
		//printf("Value of var_arr[%d] is: %d \n", x, *display);
		display++;
	}

} */

int main()
{
	//printf("%d\n", DISPLAY_HEIGHT);

	unsigned short opCode = 0;

	//4 kb of memory
	unsigned char memory[RAM_SIZE] = {0};

	unsigned char registers[REGISTER_SIZE] = {0};
	unsigned char stack[STACK_SIZE] = {0};

	unsigned short i = 0;
	unsigned short pc = 0;

	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;

	unsigned char display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = {0};
	//unsigned char *displayP[sizeof(display)];

	//keep state of keys pressed. 0 = not pressed, 1 = pressed
	unsigned char keys[NUMBER_OF_KEYS] = {0};

	/* 	int e = 0;
	for (e = 0; e < sizeof(display); e++)
	{
		displayP[e] = &display[e];
		//printf("%d\n", display[e]);
	} */

	//reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
	//write fontset to memory
	load_font(memory);
	load_rom(&memory[ADDRESS_ROM_START]);
	init_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);

	/* 	for (int x = 0; x < 80; x++)
	{
		memory[x] = fontSet[x];
	} */

	//Read rom
	/* 	unsigned char buffer[3584] = {0};
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

	for (int x = 0; x < sizeof(buffer); x++)
	{
		memory[x + 512] = buffer[x];
	} */

	//unsigned char byte0 = 0;
	//unsigned char byte1 = 0;

	pc = ADDRESS_ROM_START;

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
			pc = ADDRESS_ROM_START;
		}

		//unsigned short temp0 = 0;

		//i = opCode & 0x0FFF;
		//temp0 = opCode & 0xF000;

		switch (opCode & 0xF000)
		{
		case 0x0000:;
			//int test1 = 1;

			/* 			if (i != 0)
			{
				printf("0");
			} */

			if (opCode == 0x0E0)
			{
				printf("reset_display");
				reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
			}

			break;
		case 0x1000:
			printf("1");
			pc = opCode & 0x0FFF;
			break;
		case 0x2000:
			printf("2");
			break;
		case 0x6000:;
			printf("6");
			registers[(opCode & 0x0F00) >> 8] = opCode & 0x00FF;
			break;
		case 0x7000:
			printf("7");
			registers[(opCode & 0x0F00) >> 8] += opCode & 0x00FF;
			break;
		case 0xA000:
			printf("A");
			i = opCode & 0x0FFF;
			break;
		case 0xD000:;
			printf("D");
continue;
			unsigned char x = 0;
			unsigned char y = 0;
			unsigned char height = 0;
			unsigned char pixel = 0;
			x = (opCode & 0x0F00) >> 8;
			y = (opCode & 0x00F0) >> 4;
			height = (opCode & 0x000F);

			x = registers[x] % 64;
			y = registers[y] % 32;

			registers[0xF] = 0;
			for(int yLine = 0; yLine < height; yLine++) {
				pixel = memory[i+yLine];

				for (int xLine = 0; xLine < 8; xLine++) {

					
				}



				continue;
			}

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