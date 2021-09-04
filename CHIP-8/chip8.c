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
#include <SDL.h>


//The window we'll be rendering to
SDL_Window *window = NULL;

//The surface contained by the window
SDL_Surface *screenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface *gXOut = NULL;

// struct that handles all rendering
SDL_Renderer *renderer;

unsigned char initSDL()
{

	unsigned char success = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = 0;
	}
	else
	{
		//Create window
		//window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
		window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * 8, 32 * 8, 0);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = 0;
		}
		else
		{

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		}
	}

	return success;
}

void closeSDL()
{
	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

unsigned char loadMedia()
{

	unsigned char success = 1;

	gXOut = SDL_LoadBMP("x.bmp");

	if (gXOut == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "x.bmp", SDL_GetError());
		success = 0;
	}

	return success;
}

void draw(unsigned char *display)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// clear the current rendering target with the drawing color
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// iterating thru the display (64*32)
	//line
	for (int y = 0; y < 32; y++)
	{
		//column
		for (int x = 0; x < 64; x++)
		{
			//line 2 column 0 128+0
			if (display[x + (y * 64)])
			{
				SDL_Rect rect;

				rect.x = x * 8;
				rect.y = y * 8;
				rect.w = 8;
				rect.h = 8;

				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	// update the screen
	SDL_RenderPresent(renderer);
}

int main(int argc, char *args[])
{

	if (initSDL() == 0)
	{
		return 0;
	}

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

	//keep state of keys pressed. 0 = not pressed, 1 = pressed
	unsigned char keys[NUMBER_OF_KEYS] = {0};

	unsigned char drawFlag = 0;

	unsigned char quit = 0;

	SDL_Event e;

	//write fontset to memory
	load_font(memory);
	load_rom(&memory[ADDRESS_ROM_START]);
	init_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);

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
		pc = pc + 2;

		if (pc > 4094)
		{
			pc = ADDRESS_ROM_START;
		}

		switch (opCode & 0xF000)
		{
		case 0x0000:
			if (opCode == 0x0E0)
			{
				drawFlag = 1;
				printf("reset_display");
				reset_display(display, DISPLAY_HEIGHT * DISPLAY_WIDTH);
			}

			break;
		case 0x1000: // jump to pc
			printf("1");
			pc = opCode & 0x0FFF;
			break;
		case 0x2000:
			printf("2");
			break;
		case 0x6000:;
			printf("6");
			registers[x] = (opCode & 0x00FF);
			break;
		case 0x7000:
			printf("7");
			registers[x] += opCode & 0x00FF;
			break;
		case 0xA000:
			printf("A");
			i = opCode & 0x0FFF;
			break;
		case 0xD000:;
			printf("D");

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
		default:
			printf("Not implemented");
			break;
		}

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