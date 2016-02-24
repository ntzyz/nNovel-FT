#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdint.h>

typedef uint16_t Pixel;
typedef Pixel* Screen;

extern const int screen_width;
extern const int screen_height;

Screen create_new_screen_buffer();
void dispose_screen_buffer(Screen screen);
void apply_screen_buffer(Screen screen);

void set_pixel(Screen screen, int x, int y, Pixel pixel);

#endif //_DRAW_H_
