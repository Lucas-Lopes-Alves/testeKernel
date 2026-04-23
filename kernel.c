#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// list all the possible colors in vga mode
enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// function for setting the color in the terminal in the vga mode
// returns a size_t with the value of the foreground and background
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
    return (uint16_t)c | (uint16_t)(color << 8);
}

// VGA_MEMORY is the phisical adress to the
// the vga buffer memory
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// The terminal_buffer set as volatile to prevent otimizations
// that can ignore the acess to the memory
size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color;
volatile uint16_t *terminal_buffer = (volatile uint16_t *)VGA_MEMORY;

// it's two loops to acess all the adresses in the buffer
// and clean them, it acess like a matrix using rows and columns
// but translates it to 1D and saves in the index variable
void terminal_initialize(void)
{
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    uint16_t caractere = vga_entry(' ', terminal_color);
    for (size_t i = 0; i < VGA_HEIGHT; i++)
    {
        for (size_t j = 0; j < VGA_WIDTH; j++)
        {
            size_t index = i * VGA_WIDTH + j;
            terminal_buffer[index] = caractere;
        }
    }
}

size_t strlen(const char *str)
{
    size_t size = 0;
    while (str[size])
    {
        size++;
    }
    return size;
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

// scroll the terminal up by moving each line and
// cleaning the last lines
void terminal_scroll()
{
    for (size_t i = 1; i < VGA_HEIGHT; i++)
    {
        for (size_t j = 0; j < VGA_WIDTH; j++)
        {
            int value = (i - 1) * VGA_WIDTH + j;
            int position = i * VGA_WIDTH + j;
            terminal_buffer[value] = terminal_buffer[position];
        }
    }

    size_t line = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (size_t k = 0; k < VGA_WIDTH; k++)
    {
        terminal_buffer[k + line] = vga_entry(' ', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }
    terminal_row = VGA_HEIGHT - 1;
}

/*
 *  Writes a single character to the vga buffer
 *
 *  Handles:
 *      The '\n' character to jump to a new line
 *      If the line is the last it Triggers the Scrolls
 *      If the column is the last, it goes to a new line and the start of a new line
 */
void vga_putchar(char c, size_t size)
{
    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_scroll();
    }
    if (c == '\n')
    {
        terminal_row++;
        terminal_column = 0;

        if (terminal_row == VGA_HEIGHT)
            terminal_scroll();
        return;
    }
    size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = vga_entry(c,terminal_color);
    terminal_column++;
    if (terminal_column == VGA_WIDTH)
    {
        terminal_row++;
        terminal_column = 0;
    }
}

// function that writes in the screen and go to the next line
// when find the '\n' character, if it detects it's in the last line
// it scrolls the terminal up by invocating the terminal_scroll function
void vga_write(const char *info, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        vga_putchar(info[i],size);
    }
}

// Invokes the vga_write and handles the string's lenght
void vga_writestring(const char *string)
{
    vga_write(string, strlen(string));
}

void kernel_main(void)
{
    terminal_initialize();

    for (int i = 0; i< 25;i++)
    {
        vga_writestring("Hello kernel world\n");
    }
    while (1)
        ;
}