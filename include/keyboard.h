#ifndef __KEYBOARD__
#define __KEYBOARD__

#include <stdint.h>

char* keyboard_scancode(uint8_t key, char* out);
char* keyboard_char(const char* scancode, char* out);

#endif