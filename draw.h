#ifndef _DRAW_H_
#define _DRAW_H_

#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef uint16_t Pixel;
typedef float Alpha;
typedef Pixel* Screen;
typedef struct {
	uint8_t r, g, b;
} Color;
typedef struct {
	char *font_path;
	unsigned char* font_resource;
	size_t font_size;
	FT_Library library;
	FT_Face face;

	FT_GlyphSlot slot;
	FT_Matrix matrix;
	FT_Vector pen;
	FT_Error error;
}FONT;

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

FONT* load_font_from_file(const char *font_file_name);
void free_font(FONT *font);
void draw_text(Screen screen, int x, int y, int line_height, Pixel pixel, wchar_t *text, FONT *font);

#endif //_DRAW_H_
