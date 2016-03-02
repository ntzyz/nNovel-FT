#include <libndls.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <wchar.h>

#include "draw.h"

// screen size of nspire cx.
const int screen_width = 320;
const int screen_height = 240;

static bool is_font_loaded = false;
static unsigned char *font_resource = NULL;
static size_t font_size;
// Freetype
static FT_Library library;
static FT_Face face;

static FT_GlyphSlot slot;
static FT_Matrix matrix;
static FT_Vector pen;
static FT_Error error;

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

void load_font_from_file(const char *font_file_name) {
	if (is_font_loaded) return;

	FILE *file = fopen(font_file_name, "rb");
	if (!file) {
		printf("[ERROR] Font file does not exist: %s\n", font_file_name);
		show_msgbox("Error", "Cannot load font.");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	font_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	font_resource = (unsigned char *)malloc(font_size * sizeof (char));
	fread(font_resource, font_size, 1, file);
	is_font_loaded = true;

	fclose(file);

	error = FT_Init_FreeType( &library );

	error = FT_New_Memory_Face( library, font_resource, font_size, 0, &face );

	error = FT_Set_Char_Size( face, 12 * 64, 0, 100, 0 );

	slot = face->glyph;

	matrix.xx = (FT_Fixed)( cos(0) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(0) * 0x10000L);
	matrix.yx = (FT_Fixed)( sin(0) * 0x10000L);
	matrix.yy = (FT_Fixed)( cos(0) * 0x10000L);

}

void free_font() {
	if (is_font_loaded) {
		is_font_loaded = false;
		free(font_resource);
	}
}

void draw_bitmap(Screen screen, FT_Bitmap* bitmap, FT_Int x, FT_Int y, Pixel pixel) {
	FT_Int  i, j, p, q, c;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for ( i = x, p = 0; i < x_max; i++, p++ ) {
		for ( j = y, q = 0; j < y_max; j++, q++ ) {
			if ( i < 0 || j < 0 || i >= SCREEN_WIDTH || j >= SCREEN_HEIGHT )
				continue;

			c = bitmap->buffer[q * bitmap->width + p];

			if ( c > 0 )
				set_pixel_alpha(screen, i, j, pixel, c / 256.);
		}
	}
}

void draw_text(Screen screen, int x, int y, int line_height, Pixel pixel, wchar_t *text) {
	int n, num_chars;
	num_chars = wcslen(text);

	pen.x = 0;
	pen.y = 0;

	for (n = 0; n < num_chars; n++)
	{
		/* set transformation */
		FT_Set_Transform( face, &matrix, &pen );

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
		if ( error )
			continue;  /* ignore errors */

		/* now, draw to our target surface (convert position) */
		draw_bitmap(screen, &slot->bitmap, x + slot->bitmap_left, line_height + y - slot->bitmap_top, pixel);

		/* increment pen position */
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}
}
