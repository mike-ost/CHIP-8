#include "constants.h"

#ifndef SDL_H
#define SDL_H
unsigned char initSDL();
void closeSDL();
unsigned char loadMedia();
void draw(unsigned char *display);
int sdl_ehandler(byte_t* keyPad);
#endif
