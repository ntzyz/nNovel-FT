#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdint.h>

typedef uint16_t Pixel;
typedef float Alpha;
typedef Pixel* Screen;
typedef struct {
	uint8_t r, g, b;
} Color;

extern const int screen_width;
extern const int screen_height;

Screen create_new_screen_buffer();
void dispose_screen_buffer(Screen screen);
void apply_screen_buffer(Screen screen);

void set_pixel(Screen screen, int x, int y, Pixel pixel);
void set_pixel_alpha(Screen screen, int x, int y, Pixel pixel, Alpha alpha);

Pixel get_pixel(Screen screen, int x, int y);

Color get_color_from_pixel(Pixel pixel);
Pixel get_pixel_from_color(Color color);

#endif //_DRAW_H_
