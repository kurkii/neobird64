#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "video.h"
#include "stdio.h"
#include "../flanterm/flanterm.h"
#include "../flanterm/backends/fb.h"
#include <limine.h>


#define FORMAT_LENGTH       1
#define NORMAL              0
#define STATE_SHORT         2
#define STATE_LONG          3
#define FORMAT_SPECIFIER    1

#define LENGTH_DEFAULT      0
#define LENGTH_SHORT_SHORT  1
#define LENGTH_SHORT        2
#define LENGTH_LONG         3
#define LENGTH_LONG_LONG    4




/* 
    printf()
    params:
        string
        arguments
    
    available format specifiers:
        {i}, {d} - integer
        {s}      - string
        {c}      - char
        {k}      - color
        {n}      - newline (doesnt take in a argument)
        {x}      - base16
 */



int printf(const char *format_string, ...){

    extern struct flanterm_context *ft_ctx;
    int state = NORMAL;
    va_list a_list;
    va_start(a_list, format_string);
    for(int i = 0; i < strlen(format_string); i++){
        char current = format_string[i]; // current char in string
        switch (state){
            case NORMAL:
                switch (current) {
                    case '{':
                        state = FORMAT_SPECIFIER;
                        break;
                    default:
                        print_char(ft_ctx, current); // FAIL
                        break;                     
                }
                break;
            case FORMAT_SPECIFIER:
                switch (current) {
                    case 'n':
                        print_str(ft_ctx, "\n");
                        break;
                    case 'k':
                        print_str(ft_ctx, va_arg(a_list, char*));
                        break;
                    case 'd':
                    case 'i':
                        print_int(ft_ctx, va_arg(a_list, long long));
                        break;
                    case 's':
                        print_str(ft_ctx, va_arg(a_list, char*));
                        break;
                    case 'c':
                        ;
                        int ch = va_arg(a_list, int);
                        print_char(ft_ctx, ch);
                        break;
                    case 'x':
                        print_hex(ft_ctx, va_arg(a_list, uint64_t));
                        break;
                    case 'l':
                        current++;
                        switch (current) {
                            case 'd':
                                print_int(ft_ctx, va_arg(a_list, long long int));
                                break;
                        
                        }
                        break;
                    case '}':
                        state = NORMAL;
                        break;

                }
                break;
        }

    }
    
    va_end(a_list);


    return 0;
}