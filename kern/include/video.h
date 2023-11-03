
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
void print_hex(struct flanterm_context *ft_ctx, int num);

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

