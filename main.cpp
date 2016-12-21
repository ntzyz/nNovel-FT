#include <libndls.h>
#include <ctime>
#include <cmath>
#include <wchar.h>
#include <cstdio>

#include <sstream>
#include <fstream>
#include <codecvt>
#include <string>
#include <ios>

#include "draw.h"
#include "utf8.h"

const double pi = 3.14159265358979;

int main() {
    Screen screen;
    font_t font("/documents/ndless/wqy.ttc.tns");

    std::ifstream ifs("/documents/ndless/text.txt.tns", std::ios::in | std::ios::binary);

    ifs.seekg(0, std::ios::end);
    std::streampos size = ifs.tellg();

    ifs.seekg(0, std::ios::beg);
    char *buf = new char[size + static_cast<std::streampos>(10)];
    ifs.read(buf, size);

    show_msgbox("title", buf);

    wchar_t *ws = UTF8_to_wchar(buf);

    int fontSize = 16;
    while (!isKeyPressed(KEY_NSPIRE_ESC)) {
        if (isKeyPressed(KEY_NSPIRE_PLUS))
            fontSize++;
        if (isKeyPressed(KEY_NSPIRE_MINUS))
            fontSize--;
        if (fontSize < 0) fontSize = 0;
        screen.fillRect(Rect(0, 0, screenWidth - 1, screenHeight - 1), Color(0, 0, 0).toPixel());
        font.drawText(screen, 0, 0, fontSize, Color(0x66, 0xcc, 0xff).toPixel(), ws);
        screen.apply();
        wait_key_pressed();
    }
    // font.drawText(screen, 0, 32, 16, Color(0x66, 0xcc, 0xff).toPixel(), L"人们为什么喷 Internet Explorer (IE)？");

    delete[] ws;

    return 0;
}
