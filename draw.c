#include <libndls.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

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

FONT *load_font_from_file(const char *font_file_name) {
	FONT *font = malloc(sizeof (FONT) * 1);
	if (!font) return NULL;

	font->font_path = (char *)malloc((strlen(font_file_name) + 1) * sizeof (char));
	if (!font->font_path) {
		free(font);
		return NULL;
	}
	else {
		strcpy(font->font_path, font_file_name);
	}

	FILE *file = fopen(font->font_path, "rb");
	if (!file) {
		printf("[ERROR] Font file does not exist: %s\n", font_file_name);
		show_msgbox("Error", "Cannot load font.");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	font->font_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	font->font_resource = (unsigned char *)malloc(font->font_size * sizeof (char));
	fread(font->font_resource, font->font_size, 1, file);

	fclose(file);

	font->error = FT_Init_FreeType( &font->library );

	font->error = FT_New_Memory_Face( font->library, font->font_resource, font->font_size, 0, &font->face );

	font->error = FT_Set_Char_Size( font->face, 12 * 64, 0, 100, 0 );

	font->slot = font->face->glyph;

	font->matrix.xx = (FT_Fixed)( cos(0) * 0x10000L);
	font->matrix.xy = (FT_Fixed)(-sin(0) * 0x10000L);
	font->matrix.yx = (FT_Fixed)( sin(0) * 0x10000L);
	font->matrix.yy = (FT_Fixed)( cos(0) * 0x10000L);

	return font;
}

void free_font(FONT *font) {
	if (font) {
		free(font->font_path);
		free(font);
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

void draw_text(Screen screen, int x, int y, int line_height, Pixel pixel, wchar_t *text, FONT *font) {
	int n, num_chars;
	num_chars = wcslen(text);

	font->pen.x = 0;
	font->pen.y = 0;

	for (n = 0; n < num_chars; n++)
	{
		/* set transformation */
		FT_Set_Transform( font->face, &font->matrix, &font->pen );

		/* load glyph image into the slot (erase previous one) */
		font->error = FT_Load_Char( font->face, text[n], FT_LOAD_RENDER );
		if ( font->error )
			continue;  /* ignore errors */

		/* now, draw to our target surface (convert position) */
		draw_bitmap(screen, &font->slot->bitmap, x + font->slot->bitmap_left, line_height + y - font->slot->bitmap_top, pixel);

		/* increment pen position */
		font->pen.x += font->slot->advance.x;
		font->pen.y += font->slot->advance.y;
	}
}
