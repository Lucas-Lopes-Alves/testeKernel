#include <io.h>
#include <stdint.h>

char* keyboard_scancode(uint8_t port, char* out)
{
    uint8_t key = inb(port);
    const char digits[] = "0123456789ABCDEF";
    out[0] = digits[(key >> 4) & 0xF];
    out[1] = digits[key & 0xF];
    return out;
}