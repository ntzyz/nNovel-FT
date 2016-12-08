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

    while (*p_RTC < curr + 30) {
        i += 2 * pi / 64;
        auto c = Color(static_cast<u_int8_t>(127. * sin(i) + 127.),
                       static_cast<u_int8_t>(127. * sin(i + pi * 2 / 3) + 127.), 
                       static_cast<u_int8_t>(127. * sin(i + pi * 4 / 3) + 127.)).toPixel();
        screen.fillRect(Rect(0, 0, screenWidth - 1, screenHeight - 1), c);
        screen.apply();
        frames++;
    }

    char msg[512];
    sprintf(msg, "We had %d frames in %ld secs.", frames, *p_RTC - curr);
    show_msgbox("", msg);
    return 0;
}
