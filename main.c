#include <libndls.h>
#include <stdio.h>
#include <wchar.h>

#include "draw.h"
#include "utf8.h"

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[]) {
	assert_ndless_rev(2001);

	Screen screen = create_new_screen_buffer();

	FONT *font = load_font_from_file("/documents/ndless/wqy.ttc.tns");

	show_msgbox("Information", "wqy-microhei load successfully.");

	FONT *font2 = load_font_from_file("/documents/ndless/simkai.ttf.tns");

	show_msgbox("Information", "simkai load successfully.");

	for (int x = 0; x != screen_width; ++x)
		for (int y = 0; y != screen_height; ++y)
			set_pixel(screen, x, y, 0);

	wchar_t *text;
	char raw_text[1024];

	FILE *fp = fopen("/documents/text.tns", "r");
	fgets(raw_text, 1024, fp);
	fclose(fp);

	text = UTF8_to_wchar(raw_text);

	draw_text(screen, 0, 0, 16, 0xffff, text, font);
	draw_text(screen, 0, 20, 16, 0x1f, text, font);
	draw_text(screen, 0, 40, 16, 0x1f << 6, text, font2);
	draw_text(screen, 0, 60, 16, 0x1f << 11, text, font2);

	free(text);
	free_font(font);

	apply_screen_buffer(screen);
	wait_key_pressed();

	dispose_screen_buffer(screen);

	return 0;
}
