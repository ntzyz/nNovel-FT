#include <libndls.h>
#include <ctime>
#include <cmath>

#include "draw.h"

const double pi = 3.14159265358979;

int main() {
    Screen screen;
    font_t font("/documents/ndless/wqy.ttc.tns");
    const wchar_t *pSTR = L"静流最近好像憋不住了";

    screen.fillRect(Rect(0, 0, screenWidth - 1, screenHeight - 1), Color(0, 0, 0).toPixel());
    font.drawText(screen, 0, 0, 12, Color(255, 255, 255).toPixel(), pSTR);
    screen.apply();
    wait_key_pressed();

    return 0;
}
