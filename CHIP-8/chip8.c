#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

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
#include "inc/sdl.h"
#include <SDL.h>

/* struct CPU
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
}; */

/* void test0(struct CPU *cpu)
{
	cpu->opCode = 0;
	cpu->sp = 0;
	cpu->i = 0;
	cpu->pc = 0;
	cpu->delayTimer = 0;
	cpu->soundTimer = 0;
	cpu->drawFlag = 0;

	int a = sizeof(cpu->registers);

	for (int x = 0; x < sizeof(cpu->registers); x++)
	{
		cpu->registers[x] = 0;
	}

	for (int x = 0; x < sizeof(cpu->stack); x++)
	{
		cpu->stack[x] = 0;
	}
	
} */

int main(int argc, char *args[])
{

	//if (argc != 2) {
	//load IBM demo

	//}

	//char* rom_filename = args[1];

	if (initSDL() == 0)
	{
		return 0;
	}

	struct CPU cpu = init_cpu();
	//cpu_test(&cpu);

	/* 	struct CPU cpu;

	test0(&cpu);

	cpu.soundTimer = 0; */

	//opcode
	unsigned short opCode = 0;

	//Main memory
	unsigned char memory[RAM_SIZE] = {0};

	//CPU registers
	unsigned char registers[REGISTER_SIZE] = {0};
	//stack
	unsigned char stack[STACK_SIZE] = {0};
	//stack pointer
	unsigned short sp = 0;

	//index
	unsigned short i = 0;
	//program counter
	unsigned short pc = 0;

	//Timers
	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;

	//Array to store pixel values for display
	unsigned char display[DISPLAY_HEIGHT * DISPLAY_WIDTH] = {0};

	//keep state of keys pressed. 0 = not pressed, 1 = pressed
	unsigned char keys[NUMBER_OF_KEYS] = {0};

	//Set flag to draw to display
	unsigned char drawFlag = 0;

	//Set to quit
	unsigned char quit = 0;

	SDL_Event e;

	//write fontset to memory
	load_font(memory);
	load_rom(&memory[ADDRESS_ROM_START]);
	init_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);

	//Set program counter to start address of the ROM space
	pc = ADDRESS_ROM_START;

	unsigned short x;
	unsigned short y;

	while (quit == 0)
	{

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = 1;
			}
		}

		//emulate_cycle(&cpu, memory, display, keys);

		drawFlag = 0;

		//Each opcode is 2 bytes. Read 2 bytes from memory and combine
		opCode = (memory[pc] << 8) | memory[pc + 1]; //41514
													 //opCode = (byte0 << 8); //41514

		// Vx register, we are basically "grabbing" the x present in some
		// instructions like 3XNN
		x = (opCode & 0x0F00) >> 8;

		// Vy register, we are basically "grabbing" the y present in some
		// instructions like 5XY0
		y = (opCode & 0x00F0) >> 4;

		//Iterate program counter
		pc += 2;

		if (pc > 4094)
		{
			pc = ADDRESS_ROM_START;
		}

		switch (opCode & 0xF000)
		{
		case 0x0000:

			switch (opCode & 0x00FF)
			{
			case 0x00E0:
				//drawFlag = 1;
				printf("reset_display");
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

			/* 			if (opCode == 0x00E0)
			{
				drawFlag = 1;
				printf("reset_display");
				reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
			}
			else if (opCode == 0x00EE)
			{
				pc = stack[sp];
				sp -= 1;
			}
			else
			{
				//0nnn
				//do nothing
			} */

			break;
		case 0x1000: // jump to pc
			//printf("1");
			pc = (opCode & 0x0FFF);
			break;
		case 0x2000:
			//printf("2");
			sp++;
			pc -= 2;
			stack[sp] = pc;
			pc = (opCode & 0x0FFF);
			break;
		case 0x3000:
			if (registers[x] == (opCode & 0x00FF))
			{
				pc += 2;
			}
			break;
		case 0x4000:
			if (registers[x] != (opCode & 0x00FF))
			{
				pc += 2;
			}
			break;
		case 0x5000:
			if (registers[x] == registers[y])
			{
				pc += 2;
			}
			break;
		case 0x6000:;
			//printf("6");
			registers[x] = (opCode & 0x00FF);
			break;
		case 0x7000:
			//printf("7");
			registers[x] += opCode & 0x00FF;
			break;
		case 0x8000:
			switch (opCode & 0x000F)
			{
			case 0x0000:
				registers[x] = registers[y];
				break;
			case 0x0001:
				registers[x] |= registers[y];
				break;
			case 0x0002:
				registers[x] &= registers[y];
				break;
			case 0x0003:
				registers[x] ^= registers[y];
				break;
			case 0x0004:;
				registers[0xF] = (registers[x] + registers[y] > 0xFF) ? 1 : 0;
				registers[x] += registers[y];
				break;
			case 0x0005:
				registers[0xF] = (registers[x] > registers[y]) ? 1 : 0;
				registers[x] -= registers[y];
				break;
			case 0x0006:
				registers[0xF] = (registers[x] & 0x1) ? 1 : 0;
				registers[x] >>= 1;
				break;
			case 0x0007:
				registers[0xF] = (registers[y] > registers[x]) ? 1 : 0;
				registers[x] = registers[y] - registers[x];
				break;
			case 0x000E:
				//registers[0xF] = (registers[x] & 0x80) ? 1 : 0;
				registers[0xF] = (registers[x] >> 7) & 0x1;
				registers[x] <<= 1;
				break;
			default:
				printf("Invalid opcode");
				break;
			}
			break;
		case 0x9000:
			if (registers[x] != registers[y])
			{
				pc += 2;
			}
			break;
		case 0xA000:
			//printf("A");
			i = (opCode & 0x0FFF);
			break;
		case 0xB000:
			pc = ((opCode & 0xFFF) + registers[0]);
			break;
		case 0xC000:
			registers[x] = ((rand() % 256) & (opCode & 0x00FF));
			break;
		case 0xD000:;
			//printf("D");

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
			//continue;

			drawFlag = 1;

			unsigned char height = (opCode & 0x000F);
			unsigned char pixelValue = 0;

			//x = registers[x] % 64;
			//y = registers[y] % 32;

			registers[0xF] = 0;
			for (int yLine = 0; yLine < height; yLine++)
			{
				pixelValue = memory[i + yLine];
				//check bits in pixelValue
				for (int xLine = 0; xLine < 8; xLine++)
				{
					//check if bit in pixelValue is set to 1
					//if (pixelValue & (0x80 >> xLine) != 0)
					if ((pixelValue & (0x80 >> xLine)) != 0)
					{
						if (display[(registers[x] + xLine + ((registers[y] + yLine) * 64))] == 1)
						{
							registers[0xF] = 1;
						}
						//int aa = registers[x] + xLine + ((registers[y] + yLine) * 64);
						display[registers[x] + xLine + ((registers[y] + yLine) * 64)] ^= 1;
					}
				}
				continue;
			}

			break;
		case 0xE000:
			switch (opCode & 0x00FF)
			{
			case 0x009E:
				if (keys[registers[x]] == 1)
				{
					pc += 2;
				}
				break;
			case 0x00A1:
				if (keys[registers[x]] == 0)
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
				registers[x] = delayTimer;
				break;
			case 0x000A:
				pc -= 2;
				for (int z = 0; z < 16; z++)
				{
					if (keys[z] == 1)
					{
						registers[x] = i;
						pc += 2;
					}
				}
				break;
			case 0x0015:
				delayTimer = registers[x];
				break;
			case 0x0018:
				soundTimer = registers[x];
				break;
			case 0x001E:
				i += registers[x];
				break;
			case 0x0029:
				i = registers[x] * 5;
				break;
			case 0x0033:
				// The interpreter takes the decimal value of V[x],
				// and places the hundreds digit in memory at location in I,
				// the tens digit at location I+1, and the ones digit at
				// location I+2.
				memory[i] = registers[x] / 100;
				memory[i + 1] = registers[x] % 100 / 10;
				memory[i + 2] = registers[x] % 10;
				break;
			case 0x0055:
				for (int z = 0; z <= x; z++)
				{
					memory[i + z] = registers[z];
				}
				break;
			case 0x0065:
				for (int z = 0; z <= x; z++)
				{
					registers[z] = memory[i + z];
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