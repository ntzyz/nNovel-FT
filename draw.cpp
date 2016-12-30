#include <libndls.h>
#include <cstdio>
#include "draw.h"

// Create a new screen
Screen::Screen() {
    screenBuffer = new pixel_t[screenWidth * screenHeight];
    if (!screenBuffer) {
        // Failed on allocating memory.
    }
}

Screen::~Screen() {
    if (screenBuffer) {
        delete[] screenBuffer;
    }
}

void Screen::apply() {
    lcd_blit(screenBuffer, SCR_320x240_565);
}

void Screen::setPixel(uint32_t x, uint32_t y, pixel_t pixel) {
    pixel_t *p = screenBuffer;
    // check if the coordinate is inside of the screen.
    if (x < screenWidth && y < screenHeight) {
        p += y * screenWidth + x;
        *p = pixel;
#ifdef ENABLE_RUNTIME_WARNING
    } else {
        printf("[WARN] setPixel: Coordinate of the pixel is not valid!");
#endif
    }
}

void Screen::setPixel(Coord coord, pixel_t pixel) {
    this->setPixel(coord.x, coord.y, pixel);
}

pixel_t Screen::getPixel(uint32_t x, uint32_t y) {
    pixel_t *p = screenBuffer;
    // check if the coordinate is inside of the screen.
    if (x < screenWidth && y < screenHeight) {
        p += y * screenWidth + x;
        return *p;
    } else {
#ifdef ENABLE_RUNTIME_WARNING
        printf("[WARN] getPixel: Coordinate of the pixel is not valid!");
#endif
        return 0;
    }
}

pixel_t Screen::getPixel(Coord coord) {
    return this->getPixel(coord.x, coord.y);
}

void Screen::setPixelAlpha(uint32_t x, uint32_t y, pixel_t pixel, alpha_t alpha) {
    if (alpha < 0 || alpha > 1) {
#ifdef ENABLE_RUNTIME_WARNING
        printf("[WARN] setPixelAlpha: Opacity not valid.");
#endif
        return;
    }

    Color src_color = Color(this->getPixel(x, y));
    Color dest_color = Color(pixel);
    Color color;

    color.r = static_cast<uint8_t>(src_color.r * (1 - alpha) + dest_color.r * alpha);
    color.g = static_cast<uint8_t>(src_color.g * (1 - alpha) + dest_color.g * alpha);
    color.b = static_cast<uint8_t>(src_color.b * (1 - alpha) + dest_color.b * alpha);

    this->setPixel(x, y, color.toPixel());
}

void Screen::setPixelAlpha(Coord coord, pixel_t pixel, alpha_t alpha) {
    this->setPixelAlpha(coord.x, coord.y, pixel, alpha);
}

void Screen::fillRect(Rect rect, pixel_t pixel) {
    for (register auto x = rect.left; x <= rect.right; ++x) {
        for (register auto y = rect.top; y <= rect.bottom; ++y) {
            this->setPixel(x, y, pixel);
        }
    }
}

Color::Color(const pixel_t &pixel) {
    r = (pixel >> 11) & 0x1F;
    g = (pixel >> 5) & 0x3F;
    b = pixel & 0x1F;
}

pixel_t Color::toPixel() {
    return ((pixel_t)r << 11) | ((pixel_t)g << 5) | ((pixel_t)b);
}

#ifdef ENABLE_FREETYPE

font_t::font_t(const char *_fontPath) {
    fontPath = std::string(_fontPath);
    init();
}

font_t::font_t(const std::string &_fontPath) {
    fontPath = std::string(_fontPath);
    init();
}

font_t::~font_t() {
    if (fontRC) {
        delete[] fontRC;
        fontRC = nullptr;
    }
}

void font_t::init() {
    // Open font file.
    file = fopen(fontPath.c_str(), "rb");
    if (file == NULL) {
        return;
    }
    // Seek to end to get the size of font file.
    fseek(file, 0, SEEK_END);
    fontSize = ftell(file);

    // Reset pointer
    fseek(file, 0, SEEK_SET);

    // Allocate memory and read the blob file.
    fontRC = new uint8_t[fontSize];
    if (fontRC == nullptr) {
        return;
    }
    fread(fontRC, fontSize, 1, file);

    // Free the file handler
    fclose(file);
    file = nullptr;

    // Setup Freetype
    error = FT_Init_FreeType(&library);
    error = FT_New_Memory_Face(library, fontRC, fontSize, 0, &face);
    slot = face->glyph;

    matrix.xx = (FT_Fixed)( cos(0) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(0) * 0x10000L);
    matrix.yx = (FT_Fixed)( sin(0) * 0x10000L);
    matrix.yy = (FT_Fixed)( cos(0) * 0x10000L);
}

void font_t::drawBitmap(Screen& screen, FT_Bitmap* bitmap, FT_Int x, FT_Int y, pixel_t pixel) {
    FT_Int i, j, p, q, c;
    FT_Int xMax = x + bitmap->width;
    FT_Int yMax = y + bitmap->rows;

    for (i = x, p = 0; i < xMax; i++, p++) {
        for (j = y, q = 0; j < yMax; j++, q++) {
            c = bitmap->buffer[q * bitmap->width + p];
            if (c > 0)
                screen.setPixelAlpha(i, j, pixel, c / 256.);
        }
    }
}

void font_t::drawText(Screen& screen, int x, int y, int line_height, pixel_t pixel, const wchar_t *text) {
    int n, num_chars;
    num_chars = wcslen(text);

    pen.x = x;
    pen.y = y;
    FT_Set_Pixel_Sizes(face, 0, line_height);

    for (n = 0; n < num_chars; n++)
    {
        if (text[n] == L'\r') {
            continue;
        }
        if (text[n] == L'\n') {
            pen.x = 0; pen.y += line_height;
            continue;
        }

        FT_UInt glyph_index = FT_Get_Char_Index(face, text[n]);

        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error) continue;

        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if (error) continue;

        if (slot->bitmap_left + pen.x + slot->bitmap.width >= screenWidth) {
            pen.x = 0; pen.y += line_height;
        }

        drawBitmap(screen, &slot->bitmap, slot->bitmap_left + pen.x, line_height - slot->bitmap_top + pen.y, pixel);

        /* increment pen position */
        pen.x += slot->advance.x >> 6;
        pen.y += slot->advance.y >> 6;
    }
}

#endif
