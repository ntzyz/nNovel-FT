#include <libndls.h>
#include "draw.h"

int main() {
    Screen screen;
    for (auto x = 0; x != screenWidth; ++x) {
        for (auto y = 0; y != screenHeight; ++y) {
            screen.setPixel(x, y, Color(255, 255, 0).toPixel());
        }
    }
    screen.apply();
    wait_key_pressed();
    return 0;
}
