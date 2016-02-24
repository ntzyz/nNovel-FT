#include <libndls.h>

#include "draw.h"

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[]) {
	assert_ndless_rev(2001);

	Screen screen = create_new_screen_buffer();
	int w, h;
	for (h = 0; h != screen_height; ++h)
		for(w = 0; w != screen_width; ++w)
			switch (h / 80) {
			case 0:
				set_pixel(screen, w, h, ((Pixel)0b11111) << 11);
				break;
			case 1:
				set_pixel(screen, w, h, ((Pixel)0b111111) << 5);
				break;
			case 2:
				set_pixel(screen, w, h, (Pixel)0b11111);
				break;
			}
	apply_screen_buffer(screen);
	wait_key_pressed();
	dispose_screen_buffer(screen);

	return 0;
}