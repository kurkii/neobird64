
#pragma once
#include <stdint.h>
#include "../flanterm/flanterm.h"

typedef struct position{
    uint8_t x;
    uint8_t y;
}position;

position draw_char(position pos, struct limine_framebuffer *fb, char font[20][10]);
position draw_char_pt(position pos, struct limine_framebuffer *fb, char c);

void print_char(struct flanterm_context *ft_ctx, char c);
void print_str(struct flanterm_context *ft_ctx, char *str);
void print_int(struct flanterm_context *ft_ctx, int i);

#define COLOR_BLACK "\u001b[30m"\
#define COLOR_RED    "\u001b[31m"
#define COLOR_GREEN "\u001b[32m"
#define COLOR_YELLOW "\u001b[33m"
#define COLOR_BLUE "\u001b[34m"
#define COLOR_MAGENTA "\u001b[35m"
#define COLOR_CYAN "\u001b[36m"
#define COLOR_WHITE "\u001b[37m"

