#include<stdbool.h>
#include<stddef.h>
#include<stdint.h>

//list all the possible colors in vga mode
enum vga_color {
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

//function for setting the color in the terminal in the vga mode
//returns a size_t with the value of the foreground and background
uint16_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 4);
}

uint16_t vga_entry(unsigned char c, uint16_t color)
{
    return (uint16_t) c | (uint16_t)(color << 8);
}

//macros for vga width and height and the memory
//where we write the text
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;


void terminal_initialize(void)
{

    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t i = 0; i < VGA_HEIGHT; i++)
    {
        for (size_t j = 0; j< VGA_WIDTH; j++)
        {
            uint16_t caractere = vga_entry(' ', terminal_color);
            size_t index = i * VGA_WIDTH + j;
            terminal_buffer[index] = caractere;
        }
    }
}

size_t strlen(const char* str)
{
    size_t size = 0;
    while(str[size])
    {
        size++;
    }
    return size;

}

void vga_write(const char* info, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {

        if (info[i] == '\n')
        {
            terminal_column = 0;
            terminal_row++;

            if (terminal_row == VGA_HEIGHT)
                terminal_row = 0;

            continue;
        }

        size_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = vga_entry(info[i],terminal_color);
        terminal_column++;
        if (terminal_column == VGA_WIDTH)
        {
            terminal_column = 0;
            terminal_row++;
        }

        if (terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

void main(void)
{
    char palavra[] = "teste";
    int tamanho = strlen(palavra);
    terminal_initialize();

    vga_write(palavra,strlen(palavra));
    vga_write("ola",strlen("ola"));

    while(1);
}