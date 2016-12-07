#include "utf8.h"

#include <string>
#include <limits>

#include <cstdlib>

/**
 * From http://www.cplusplus.com/forum/general/31270/
 * Will be rewrite later.
 **/
std::wstring FromUTF8(const char* str)
{
    const unsigned char* s = reinterpret_cast<const unsigned char*>(str);

    static const wchar_t badchar = '?';

    std::wstring ret;

    unsigned i = 0;
    while(s[i])
    {
        try
        {
            if(s[i] < 0x80)         // 00-7F: 1 byte codepoint
            {
                ret += s[i];
                ++i;
            }
            else if(s[i] < 0xC0)    // 80-BF: invalid for midstream
                throw 0;
            else if(s[i] < 0xE0)    // C0-DF: 2 byte codepoint
            {
                if((s[i+1] & 0xC0) != 0x80)        throw 1;

                ret +=  ((s[i  ] & 0x1F) << 6) |
                        ((s[i+1] & 0x3F));
                i += 2;
            }
            else if(s[i] < 0xF0)    // E0-EF: 3 byte codepoint
            {
                if((s[i+1] & 0xC0) != 0x80)        throw 1;
                if((s[i+2] & 0xC0) != 0x80)        throw 2;

                wchar_t ch =
                        ((s[i  ] & 0x0F) << 12) |
                        ((s[i+1] & 0x3F) <<  6) |
                        ((s[i+2] & 0x3F));
                i += 3;

                // make sure it isn't a surrogate pair
                if((ch & 0xF800) == 0xD800)
                    ch = badchar;

                ret += ch;
            }
            else if(s[i] < 0xF8)    // F0-F7: 4 byte codepoint
            {
                if((s[i+1] & 0xC0) != 0x80)        throw 1;
                if((s[i+2] & 0xC0) != 0x80)        throw 2;
                if((s[i+3] & 0xC0) != 0x80)        throw 3;

                unsigned long ch =
                        ((s[i  ] & 0x07) << 18) |
                        ((s[i+1] & 0x3F) << 12) |
                        ((s[i+2] & 0x3F) <<  6) |
                        ((s[i+3] & 0x3F));
                i += 4;

                // make sure it isn't a surrogate pair
                if((ch & 0xFFF800) == 0xD800)
                    ch = badchar;

                if(ch < 0x10000)    // overlong encoding -- but technically possible
                    ret += static_cast<wchar_t>(ch);
                else if(std::numeric_limits<wchar_t>::max() < 0x110000)
                {
                    // wchar_t is too small for 4 byte code point
                    //  encode as UTF-16 surrogate pair

                    ch -= 0x10000;
                    ret += static_cast<wchar_t>( (ch >> 10   ) | 0xD800 );
                    ret += static_cast<wchar_t>( (ch & 0x03FF) | 0xDC00 );
                }
                else
                    ret += static_cast<wchar_t>(ch);
            }
            else                    // F8-FF: invalid
                throw 0;
        }
        catch(int skip)
        {
            if(!skip)
            {
                do
                {
                    ++i;
                }while((s[i] & 0xC0) == 0x80);
            }
            else
                i += skip;
        }
    }

    return ret;
}

wchar_t *UTF8_to_wchar(const char *str) {
    std::wstring ws = FromUTF8(str);
    wchar_t *pws = (wchar_t *)malloc (sizeof (wchar_t) * (ws.length() + 1));
    wcscpy(pws, ws.c_str());
    return pws;
}
