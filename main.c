#include <libndls.h>

#include "draw.h"

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[]) {
	assert_ndless_rev(2001);

	Screen screen = create_new_screen_buffer();

	load_font_from_file("/documents/ndless/DroidSans.ttf.tns");

	for (int x = 0; x != screen_width; ++x)
		for (int y = 0; y != screen_height; ++y)
			set_pixel(screen, x, y, 0);

	draw_text(screen, 0, 0, 16, 0xffff, L"Text");
	draw_text(screen, 0, 16, 16, 0x1f, L"Text");
	draw_text(screen, 0, 32, 16, 0x1f << 6, L"Text");
	draw_text(screen, 0, 48, 16, 0x1f << 11, L"Text");
	free_font();

	apply_screen_buffer(screen);
	wait_key_pressed();

	dispose_screen_buffer(screen);

	return 0;
}
