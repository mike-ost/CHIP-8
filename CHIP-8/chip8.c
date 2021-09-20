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
			//Clear the display.
			reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
			break;
		case 0x00EE:
			// The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
			pc = stack[sp];
			pc += 2;
			sp--;
			break;
		default:
			//printf("Invalid opcode");
			break;
		}
		break;
	case 0x1000:
		// jump to pc The interpreter sets the program counter to nnn.
		pc = (opCode & 0x0FFF);
		break;
	case 0x2000:
		// The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
		sp++;
		pc -= 2;
		stack[sp] = pc;
		pc = (opCode & 0x0FFF);
		break;
	case 0x3000:
		// The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
		if (V[x] == (opCode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x4000:
		// The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
		if (V[x] != (opCode & 0x00FF))
		{
			pc += 2;
		}
		break;
	case 0x5000:
		// The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
		if (V[x] == V[y])
		{
			pc += 2;
		}
		break;
	case 0x6000:;
		// The interpreter puts the value kk into register Vx.
		V[x] = (opCode & 0x00FF);
		break;
	case 0x7000:
		// Adds the value kk to the value of register Vx, then stores the result in Vx.
		V[x] += (opCode & 0x00FF);
		break;
	case 0x8000:
		switch (opCode & 0x000F)
		{
		case 0x0000:
			// Stores the value of register Vy in register Vx.
			V[x] = V[y];
			break;
		case 0x0001:
			// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
			V[x] |= V[y];
			break;
		case 0x0002:
			// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
			V[x] &= V[y];
			break;
		case 0x0003:
			// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
			V[x] ^= V[y];
			break;
		case 0x0004:;
			// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
			V[0xF] = ((V[x] + V[y]) > 0xFF) ? 1 : 0;
			V[x] += V[y];
			break;
		case 0x0005:
			// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
			V[0xF] = (V[x] > V[y]) ? 1 : 0;
			V[x] -= V[y];
			break;
		case 0x0006:
			// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
			V[0xF] = (V[x] & 0x1) ? 1 : 0;
			V[x] >>= 1;
			break;
		case 0x0007:
			// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
			V[0xF] = (V[y] > V[x]) ? 1 : 0;
			V[x] = (V[y] - V[x]);
			break;
		case 0x000E:
			// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
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
		// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
		if (V[x] != V[y])
		{
			pc += 2;
		}
		break;
	case 0xA000:
		// The value of register I is set to nnn.
		I = (opCode & 0x0FFF);
		break;
	case 0xB000:
		// The program counter is set to nnn plus the value of V0.
		pc = ((opCode & 0xFFF) + V[0]);
		break;
	case 0xC000:
		// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
		V[x] = ((rand() % 256) & (opCode & 0x00FF));
		break;
	case 0xD000:;

		/* 	Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen.
 If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
  See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites. */

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
			// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
			if (keyPad[V[x]] == 1)
			{
				pc += 2;
			}
			break;
		case 0x00A1:
			// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
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
			// The value of DT is placed into Vx.
			V[x] = delayTimer;
			break;
		case 0x000A:
			// All execution stops until a key is pressed, then the value of that key is stored in Vx.
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
			//DT is set equal to the value of Vx.
			delayTimer = V[x];
			break;
		case 0x0018:
			// ST is set equal to the value of Vx.
			soundTimer = V[x];
			break;
		case 0x001E:
			// The values of I and Vx are added, and the results are stored in I.
			I += V[x];
			break;
		case 0x0029:
			// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
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
			//Store registers V0 through Vx in memory starting at location I.

			//The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
			for (int z = 0; z <= x; z++)
			{
				memory[(I + z)] = V[z];
			}
			break;
		case 0x0065:
			//Read registers V0 through Vx from memory starting at location I.

			//The interpreter reads values from memory starting at location I into registers V0 through Vx.
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