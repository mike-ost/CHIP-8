#include "inc/display.h"

void init_display(unsigned char *display, int size) {
	reset_display(display, size);
}

void reset_display(unsigned char *display, int size) {

	for (int x = 0; x < size; x++) {
		*display = 0;
		//printf("Value of var_arr[%d] is: %d \n", x, *display);
		display++;
	}

}