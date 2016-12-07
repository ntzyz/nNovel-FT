#include <libndls.h>
#include <ctime>
#include <cmath>

#include "draw.h"

const double pi = 3.14159265358979;

int main() {
    Screen screen;
    double i;

    long * const p_RTC = (long*)0x90090000;
    auto curr = *p_RTC;
    int frames = 0;
    while (curr == *p_RTC);
    curr = *p_RTC;

    while (*p_RTC < curr + 5) {
        i += 2 * pi / 32;
        auto c = Color(static_cast<u_int8_t>(128. * sin(i) + 128.),
                       static_cast<u_int8_t>(128. * sin(i + pi * 2 / 3) + 128.), 
                       static_cast<u_int8_t>(128. * sin(i + pi * 4 / 3) + 128.)).toPixel();
        for (auto x = 0; x != screenWidth; ++x) {
            for (auto y = 0; y != screenHeight; ++y) {
                screen.setPixel(x, y, c);
            }
        }
        screen.apply();
        frames++;
    }
    char msg[512];
    sprintf(msg, "We had %d frames in 5 secs.", frames);
    show_msgbox("", msg);
    return 0;
}
