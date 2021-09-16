#include <stdio.h>
#include <errno.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "inc/chip8.h"
#include "inc/display.h"
#include "inc/font.h"
#include "inc/loadrom.h"
#include "inc/sdl.h"
#include <SDL.h>

void run_emualtion_cycle();

//opcode
num_t opCode = 0;
//Main memory
byte_t memory[RAM_SIZE] = {0};
//CPU registers
byte_t V[REGISTER_SIZE] = {0};
//stack
num_t stack[STACK_SIZE] = {0};
//stack pointer
num_t sp = 0;
//I
num_t I = 0;
//program counter
num_t pc = 0;
//Array to store pixel values for display
byte_t display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = {0};
//keep state of keyPad pressed. 0 = not pressed, 1 = pressed
byte_t keyPad[NUMBER_OF_KEYS] = {0};

//Timers
unsigned char delayTimer = 0;
unsigned char soundTimer = 0;

//Set flag to draw to display
unsigned char drawFlag = 0;

//Set to quit
unsigned char quit = 0;


int main(int argc, char *args[])
{
	//Seed random number generator with timestamp
	srand((unsigned int)time(NULL));

	//if (argc != 2) {
	//load IBM demo
	//}

	//char* rom_filename = args[1];

	if (initSDL() == 0)
	{
		return 0;
	}

	//write fontset to memory
	load_font(memory);
	load_rom(&memory[ADDRESS_ROM_START]);
	init_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);

	//Set program counter to start address of the ROM space
	pc = ADDRESS_ROM_START;

	//Main loop
	while (quit == 0)
	{
		quit = sdl_ehandler(keyPad);

		run_emualtion_cycle();

		/*
     * Update timers:
     *
     * Decrement timers if they are > 0
     * */
		if (delayTimer > 0)
		{
			delayTimer -= 1;
		}
		if (soundTimer > 0)
		{
			//sound_flag = 1;
			//printf("BEEP\n");
			soundTimer -= 1;
		}

		//Draw to display
		if (drawFlag == 1)
		{
			draw(display);
		}

		Sleep(10);
	}

	printf("EXIT");

	closeSDL();
	return 0;
}

void run_emualtion_cycle()
{

	//Reset draw flag
	drawFlag = 0;

	//Fetch opcode
	//Each opcode is 2 bytes. Read 2 bytes from memory and combine
	opCode = (memory[pc] << 8) | memory[(pc + 1)]; //41514
												   //opCode = (byte0 << 8); //41514

	// Vx register, we are basically "grabbing" the x present in some
	// instructions like 3XNN
	num_t x = (opCode & 0x0F00) >> 8;

	// Vy register, we are basically "grabbing" the y present in some
	// instructions like 5XY0
	num_t y = (opCode & 0x00F0) >> 4;

	//Iterate program counter
	pc += 2;

	switch (opCode & 0xF000)
	{
	case 0x0000:

		switch (opCode & 0x00FF)
		{
		case 0x00E0:
			reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
			break;
		case 0x00EE:
			pc = stack[sp];
			pc += 2;
			sp--;
			break;
		default:
			//printf("Invalid opcode");
			break;
		}
		break;
	case 0x1000: // jump to pc
		pc = (opCode & 0x0FFF);
		break;
	case 0x2000:
		sp++;
		pc -= 2;
		stack[sp] = pc;
		pc = (opCode & 0x0FFF);
		break;
	case 0x3000:
		if (V[x] == (opCode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x4000:
		if (V[x] != (opCode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x5000:
		if (V[x] == V[y])
		{
			pc += 2;
		}
		break;
	case 0x6000:;
		V[x] = (opCode & 0x00FF);
		break;
	case 0x7000:
		V[x] += (opCode & 0x00FF);
		break;
	case 0x8000:
		switch (opCode & 0x000F)
		{
		case 0x0000:
			V[x] = V[y];
			break;
		case 0x0001:
			V[x] |= V[y];
			break;
		case 0x0002:
			V[x] &= V[y];
			break;
		case 0x0003:
			V[x] ^= V[y];
			break;
		case 0x0004:;
			V[0xF] = ((V[x] + V[y]) > 0xFF) ? 1 : 0;
			V[x] += V[y];
			break;
		case 0x0005:
			V[0xF] = (V[x] > V[y]) ? 1 : 0;
			V[x] -= V[y];
			break;
		case 0x0006:
			V[0xF] = (V[x] & 0x1) ? 1 : 0;
			V[x] >>= 1;
			break;
		case 0x0007:
			V[0xF] = (V[y] > V[x]) ? 1 : 0;
			V[x] = (V[y] - V[x]);
			break;
		case 0x000E:
			//V[0xF] = (V[x] & 0x80) ? 1 : 0;
			V[0xF] = (V[x] >> 7) & 0x1;
			V[x] <<= 1;
			break;
		default:
			printf("Invalid opcode");
			break;
		}
		break;
	case 0x9000:
		if (V[x] != V[y])
		{
			pc += 2;
		}
		break;
	case 0xA000:
		I = (opCode & 0x0FFF);
		break;
	case 0xB000:
		pc = ((opCode & 0xFFF) + V[0]);
		break;
	case 0xC000:
		V[x] = ((rand() % 256) & (opCode & 0x00FF));
		break;
	case 0xD000:;

		/* 
            Draws a sprite at coordinate (V[x], V[y])
            that has a width of 8 pixels and a 
            height of N pixels. Each row of 
            8 pixels is read as bit-coded starting
            from memory location I; I value doesn't
            change after the execution of this instruction.
            As described above, V[F] is set to 1 
            if any screen pixels are flipped from set
            to unset when the sprite is
            drawn, and to 0 if that doesn't happen.
            */

		drawFlag = 1;

		unsigned char height = (opCode & 0x000F);
		unsigned char pixelValue = 0;

		V[0xF] = 0;
		for (int yLine = 0; yLine < height; yLine++)
		{
			pixelValue = memory[(I + yLine)];
			//check bits in pixelValue
			for (int xLine = 0; xLine < 8; xLine++)
			{
				//check if bit in pixelValue is set to 1
				//if (pixelValue & (0x80 >> xLine) != 0)
				if ((pixelValue & (0x80 >> xLine)) != 0)
				{
					if (display[(V[x] + xLine + ((V[y] + yLine) * 64))] == 1)
					{
						V[0xF] = 1;
					}
					display[V[x] + xLine + ((V[y] + yLine) * 64)] ^= 1;
				}
			}
			continue;
		}

		break;
	case 0xE000:
		switch (opCode & 0x00FF)
		{
		case 0x009E:
			if (keyPad[V[x]] == 1)
			{
				pc += 2;
			}
			break;
		case 0x00A1:
			if (keyPad[V[x]] == 0)
			{
				pc += 2;
			}
			break;
		default:
			printf("Invalid opcode");
			break;
		}
		break;
	case 0xF000:
		switch (opCode & 0x00FF)
		{
		case 0x0007:
			V[x] = delayTimer;
			break;
		case 0x000A:
			pc -= 2;
			for (int z = 0; z < 16; z++)
			{
				if (keyPad[z] == 1)
				{
					V[x] = I;
					pc += 2;
				}
			}
			break;
		case 0x0015:
			delayTimer = V[x];
			break;
		case 0x0018:
			soundTimer = V[x];
			break;
		case 0x001E:
			I += V[x];
			break;
		case 0x0029:
			I = (V[x] * 5);
			break;
		case 0x0033:
			// The interpreter takes the decimal value of V[x],
			// and places the hundreds digit in memory at location in I,
			// the tens digit at location I+1, and the ones digit at
			// location I+2.
			memory[I] = (V[x] / 100);
			memory[I + 1] = ((V[x] % 100) / 10);
			memory[I + 2] = (V[x] % 10);
			break;
		case 0x0055:
			for (int z = 0; z <= x; z++)
			{
				memory[(I + z)] = V[z];
			}
			break;
		case 0x0065:
			for (int z = 0; z <= x; z++)
			{
				V[z] = memory[(I + z)];
			}
			break;
		default:
			printf("Invalid opcode");
			break;
		}
		break;
	default:
		printf("Invalid opcode");
		break;
	}
}