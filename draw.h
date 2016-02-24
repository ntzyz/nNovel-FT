#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdint.h>

typedef uint16_t Pixel;
typedef Pixel* Screen;

Screen create_screen_buffer();
void dispose_screen_buffer(Screen screen);

void set_pixel();

#endif //_DRAW_H_
