#pragma once
#include <cstdint>

using pixel_t=uint16_t;
using alpha_t=float;

const size_t screenWidth = 320;
const size_t screenHeight = 240;

class Screen {
private:
    pixel_t *screenBuffer = nullptr;
public:
    Screen();
    ~Screen();

    void apply();
    void setPixel(size_t x, size_t y, pixel_t pixel);
    pixel_t getPixel(size_t x, size_t y);
    void setPixelAlpha(size_t x, size_t y, pixel_t pixel, alpha_t alpha);
};

class Color {
public:
    uint8_t r, g, b;
    Color() : r(0), g(0), b(0) {};
    Color(const uint8_t &_r, const uint8_t &_g, const uint8_t &_b) : r(_r), g(_g), b(_b) {};
    Color(const pixel_t &pixel);
    pixel_t toPixel();
};