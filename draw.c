#include <libndls.h>
#include <stdlib.h>

#include "draw.h"

// screen size of nspire cx.
const int screen_width = 320;
const int screen_height = 240;

// create a screen buffer for other drawing function.
Screen create_new_screen_buffer() {
	Screen screen = malloc(screen_width  * screen_height * sizeof (Pixel));
	if (!screen) {
		// malloc failure.
		show_msgbox("Error", "Cannot allocate memory.");
		exit(1);
	}
	return screen;
}

void dispose_screen_buffer(Screen screen) {
	// just free it if not null
	if (screen) {
		free(screen);
	}
}

void apply_screen_buffer(Screen screen) {
	static Screen vram = NULL;
	if (!vram) {
		vram = SCREEN_BASE_ADDRESS;
	}
	if (screen) {
		memcpy(vram, screen, screen_width * screen_height * sizeof (Pixel));
	}
}

void set_pixel(Screen screen, int x, int y, Pixel pixel) {
	// check if the coordinate is inside of the screen.
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
		screen += y * screen_width + x;
		*screen = pixel;
	} else {
		printf("[WARN] Coordinate of the pixel is not valid!");
	}
}
