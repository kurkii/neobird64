#include <limine.h>
#include "../flanterm/flanterm.h"
#include "video.h"
#include <string.h>
#define MAX_INTERGER_SIZE 128

void print_char(struct flanterm_context *ft_ctx, char c){
    flanterm_write(ft_ctx, &c, 1);
}

void print_str(struct flanterm_context *ft_ctx, char *str){
    flanterm_write(ft_ctx, str, strlen(str));
}

void print_int(struct flanterm_context *ft_ctx, uint64_t num){
    int arr[MAX_INTERGER_SIZE];
    int j = 0;
    int digitscount = 0;

    while(num != 0){
        int mod = num % 10;
        arr[j] = mod;
        num /= 10;
        j++;

        if(j == MAX_INTERGER_SIZE){            // ub handling like any good programmer
                return;
        }

        digitscount = j;
        if(num == 0){
                arr[j] = mod;
        }
    }
    for(int i = digitscount-1; i != -1; i--){
        switch(arr[i]){
                case 1:
                        print_char(ft_ctx, '1');
                        break;
                case 2:
                        print_char(ft_ctx, '2');
                        break;
                case 3:
                        print_char(ft_ctx, '3');
                        break;
                case 4:
                        print_char(ft_ctx, '4');
                        break;
                case 5:
                        print_char(ft_ctx, '5');
                        break;
                case 6:
                        print_char(ft_ctx, '6');
                        break;
                case 7:
                        print_char(ft_ctx, '7');
                        break;
                case 8:
                        print_char(ft_ctx, '8');
                        break;
                case 9:
                        print_char(ft_ctx, '9');
                        break;
                case 0:
                        print_char(ft_ctx, '0');
                        break;
        }
                
    }
    if(digitscount == 0){
        print_char(ft_ctx, '0');
    }
}

void print_hex(struct flanterm_context *ft_ctx, uint64_t num){
    int arr[MAX_INTERGER_SIZE];
    int j = 0;
    int digitscount = 0;

    while(num != 0){
        int mod = num % 16;
        arr[j] = mod;
        num /= 16;
        j++;

        if(j == 64){            // ub handling like any good programmer
                return;
        }

        digitscount = j;
        if(num == 0){
                arr[j] = mod;
        }
    }
    for(int i = digitscount-1; i != -1; i--){
        switch(arr[i]){
                case 1:
                        print_char(ft_ctx, '1');
                        break;
                case 2:
                        print_char(ft_ctx, '2');
                        break;
                case 3:
                        print_char(ft_ctx, '3');
                        break;
                case 4:
                        print_char(ft_ctx, '4');
                        break;
                case 5:
                        print_char(ft_ctx, '5');
                        break;
                case 6:
                        print_char(ft_ctx, '6');
                        break;
                case 7:
                        print_char(ft_ctx, '7');
                        break;
                case 8:
                        print_char(ft_ctx, '8');
                        break;
                case 9:
                        print_char(ft_ctx, '9');
                        break;
                case 10:
                        print_char(ft_ctx, 'A');
                        break;
                case 11:
                        print_char(ft_ctx, 'B');
                        break;
                case 12:
                        print_char(ft_ctx, 'C');
                        break;
                case 13:
                        print_char(ft_ctx, 'D');
                        break;
                case 14:
                        print_char(ft_ctx, 'E');
                        break;
                case 15:
                        print_char(ft_ctx, 'F');
                        break;
                case 0:
                        print_char(ft_ctx, '0');
                        break;
        }
                
    }
    if(digitscount == 0){
        print_char(ft_ctx, '0');
    }
}

char toupper(char c){
        switch(c){
                case 'a':
                        return 'A';
                case 'b':
                        return 'B';
                case 'c':
                        return 'C';
                case 'd':
                        return 'D';
                case 'e':
                        return 'E';
                case 'f':
                        return 'F';
                case 'g':
                        return 'G';
                case 'h':
                        return 'H';
                case 'i':
                        return 'I';
                case 'j':
                        return 'J';
                case 'k':
                        return 'K';
                case 'l':
                        return 'L';
                case 'm':
                        return 'M';
                case 'n':
                        return 'N';
                case 'o':
                        return 'O';
                case 'p':
                        return 'P';
                case 't':
                        return 'T';
                case 'r':
                        return 'R';
                case 's':
                        return 'S';
                case 'u':
                        return 'U';
                case 'v':
                        return 'V';
                case 'w':
                        return 'W';
                case 'x':
                        return 'X';
                case 'y':
                        return 'Y';
                case 'z':
                        return 'Z';
                default:
                        return c;

        }
                
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
