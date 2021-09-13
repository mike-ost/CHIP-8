#include <stdio.h>
//#include <stdlib.h>
#include <errno.h>

#include "inc/loadrom.h"

void load_rom(unsigned char *memory)
{
    unsigned char buffer[3584] = {0};
    //printf("%d\n", STACK_SIZE);
    //Read rom
    FILE *fptr;
    //errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\ibm.ch8", "rb");
    errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\Pong (alt).ch8", "rb");
    //errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\Chip8 emulator Logo [Garstyciuks].ch8", "rb");
    //errno_t err = fopen_s(&fptr, "C:\\temp\\roms\\breakout.ch8", "rb");
    // err = fopen_s(&fptr, "C:\\temp\\roms\\jason.ch8", "rb");
    // err = fopen_s(&fptr, "C:\\temp\\roms\\test_opcode.ch8", "rb");

    if (err || fptr == NULL)
    {
        printf("FILE ERROR!");
        //TODO Handle error
        //return 0;
    }
    else
    {
        //fread_s(buffer, sizeof(buffer), 1, sizeof(buffer), fptr);
        fread(buffer, sizeof(buffer), 1, fptr);
    }

    fclose(fptr);

    for (int x = 0; x < sizeof(buffer); x++)
    {
        *memory = buffer[x];
        memory++;
    }
}