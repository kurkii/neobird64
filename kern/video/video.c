#include "../limine.h"
#include "../flanterm/flanterm.h"
#include "video.h"
#include <string.h>
#define FONT_SPACE 10

void print_char(struct flanterm_context *ft_ctx, char c){
    flanterm_write(ft_ctx, &c, 1);
}

void print_str(struct flanterm_context *ft_ctx, char *str){
    flanterm_write(ft_ctx, str, strlen(str));
}

void print_int(struct flanterm_context *ft_ctx, int i){
    flanterm_write(ft_ctx, (char*)i, 1);
}

/* position draw_char(position pos, struct limine_framebuffer *fb, char font[20][10]){
    uint32_t *fb_ptr = fb->address;
    int offset = pos.x * FONT_SPACE;

    for(int i, j = 0; j < FONT_HEIGHT; i++){

        if(font[j][i] == 1){
            fb_ptr[offset+j * (fb->pitch / 4) + i] = 0xffffff;
        }
        if(i == FONT_WIDTH){
            j++;
            i = 0;
        }

    }
    pos.x += 1;
    
    return pos;
} */

/* position draw_char_pt(position pos, struct limine_framebuffer *fb, char c){
    uint32_t *fb_ptr = fb->address;
    int offset = pos.x * FONT_SPACE;

    if (c < ' ')
        c = 0;
    else 
        c -= ' ';
    const unsigned char* chr = font[c];
    
    // Draw pixels
    for (int j = 0; j < FONT_WIDTH; j++)
        for (int i = 0; i < FONT_CHARS; i++)
            if (chr[j] & (1 << i)){
                fb_ptr[offset+i * (fb->pitch / 4) + j] = 0xffffff;
                
            }
            
    
    pos.x += 1;
    return pos;
} */
