#ifndef _UTF_8_H
#define _UTF_8_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t *UTF8_to_wchar(const char *str);

#ifdef __cplusplus
}
#endif

#endif
