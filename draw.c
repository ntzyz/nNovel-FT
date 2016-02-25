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
		printf("[WARN] set_pixel: Coordinate of the pixel is not valid!");
	}
}

Color get_color_from_pixel(Pixel pixel) {
	Color color;

	color.r = pixel >> 11;
	color.g = pixel >> 6;
	color.b = pixel;

	color.r &= 0x1F;
	color.g &= 0x1F;
	color.b &= 0x1F;

	return color;
}

Pixel get_pixel(Screen screen, int x, int y) {
	// check if the coordinate is inside of the screen.
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
		screen += y * screen_width + x;
		return *screen;
	} else {
		printf("[WARN] get_pixel: Coordinate of the pixel is not valid!");
		return 0;
	}
}

/*
 * aphpa 浮点，取值范围 [0,1]，1为完全不透明
 */
void set_pixel_alpha(Screen screen, int x, int y, Pixel pixel, Alpha alpha) {
	if (alpha < 0 || alpha > 1) {
		printf("[WARN] set_pixel_alpha: Opacity not valid.");
		return;
	}
	else if (x < 0 || x >= screen_width || y < 0 || y >= screen_height) {
		printf("[WARN] get_pixel: Coordinate of the pixel is not valid!");
		return;
	}

	Color src_color = get_color_from_pixel(get_pixel(screen, x, y));
	Color dest_color = get_color_from_pixel(pixel);
	Color color;

	color.r = (int)(src_color.r * (1 - alpha) + dest_color.r * alpha);
	color.g = (int)(src_color.g * (1 - alpha) + dest_color.g * alpha);
	color.b = (int)(src_color.b * (1 - alpha) + dest_color.b * alpha);

	set_pixel(screen, x, y, get_pixel_from_color(color));
}

Pixel get_pixel_from_color(Color color) {
	color.r &= 0x1F;
	color.g &= 0x1F;
	color.b &= 0x1F;

	return ((uint16_t)color.r << 11) | ((uint16_t)color.g << 6) | ((uint16_t)color.b);
}
