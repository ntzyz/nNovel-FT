#include <libndls.h>
#include <ctime>
#include <cmath>
#include <wchar.h>
#include <cstdio>

#include <sstream>
#include <fstream>
#include <codecvt>
#include <string>

#include "draw.h"
#include "utf8.h"

const double pi = 3.14159265358979;

int main() {
    Screen screen;
    font_t font("/documents/ndless/wqy.ttc.tns");

    FILE *fp = fopen("/documents/ndless/text.txt.tns", "rb");
    char buf[512];
    fgets(buf, 512, fp);
    show_msgbox("title", buf);
    std::wstring ws = UTF8_to_wchar(buf);

    screen.fillRect(Rect(0, 0, screenWidth - 1, screenHeight - 1), Color(0, 0, 0).toPixel());
    font.drawText(screen, 0, 0, 16, Color(0x66, 0xcc, 0xff).toPixel(), ws.c_str());
    font.drawText(screen, 0, 32, 16, Color(0x66, 0xcc, 0xff).toPixel(), L"人们为什么喷 Internet Explorer (IE)？");

    screen.apply();
    wait_key_pressed();

    return 0;
}
