#include <stdio.h>
#include "inc/sdl.h"
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
		window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_WIDTH * 8, DISPLAY_HEIGHT * 8, 0);
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