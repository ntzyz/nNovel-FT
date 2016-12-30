#pragma once
#include <cstdint>
#include <string>

using pixel_t=uint16_t;
using alpha_t=float;

const size_t screenWidth = 320;
const size_t screenHeight = 240;

class Rect {
public:
    uint32_t top, left, right, bottom;
    Rect(const uint32_t& _top, const uint32_t& _left, const uint32_t& _right, const uint32_t& _bottom) :
        top(_top), left(_left), right(_right), bottom(_bottom) {};
};

class Coord {
public:
    uint32_t x, y;
    Coord(const uint32_t& _x,const uint32_t& _y):
        x(_x), y(_y) {};
};

class Screen {
private:
    pixel_t *screenBuffer = nullptr;
public:
    Screen();
    ~Screen();
    void apply();
    void setPixel(uint32_t x, uint32_t y, pixel_t pixel);
    void setPixel(Coord coord, pixel_t pixel);
    pixel_t getPixel(Coord coord);
    pixel_t getPixel(uint32_t x, uint32_t y);
    void setPixelAlpha(Coord coord, pixel_t pixel, alpha_t alpha);
    void setPixelAlpha(uint32_t x, uint32_t y, pixel_t pixel, alpha_t alpha);
    void fillRect(Rect rect, pixel_t pixel);
};

class Color {
public:
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {};
    Color(const uint8_t &_r, const uint8_t &_g, const uint8_t &_b) :
        r(_r >> 3), g(_g >> 2), b(_b >> 3) {};
    Color(const pixel_t &pixel);
    pixel_t toPixel();
};


#ifdef ENABLE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H

class font_t {
private:
    std::string fontPath;
    FILE *file;
    uint8_t *fontRC;
    size_t fontSize;
    FT_Library library;
    FT_Face face;
    FT_GlyphSlot slot;
    FT_Matrix matrix;
    FT_Vector pen;
    FT_Error error;
    void init();
    void drawBitmap(Screen& screen, FT_Bitmap* bitmap, FT_Int x, FT_Int y, pixel_t pixel);
public:
    font_t(const char *_fontPath);
    font_t(const std::string &_fontPath);
    ~font_t();
    void drawText(Screen& screen, int x, int y, int line_height, pixel_t pixel, const wchar_t *text);
};

#endif
