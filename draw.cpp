#include <cstring>
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
    } else {
        printf("[WARN] setPixel: Coordinate of the pixel is not valid!");
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
        printf("[WARN] getPixel: Coordinate of the pixel is not valid!");
        return 0;
    }
}

pixel_t Screen::getPixel(Coord coord) {
    return this->getPixel(coord.x, coord.y);
}

void Screen::setPixelAlpha(uint32_t x, uint32_t y, pixel_t pixel, alpha_t alpha) {
    if (alpha < 0 || alpha > 1) {
        printf("[WARN] setPixelAlpha: Opacity not valid.");
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
