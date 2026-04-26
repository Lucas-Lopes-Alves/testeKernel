#include "terminal.h"
#include "keyboard.h"

void kernel_main(void)
{
    terminal_initialize();
    while (1)
    {
        char out[3];
        keyboard_scancode(0x60,out);
        vga_writestring(out);
    }
}