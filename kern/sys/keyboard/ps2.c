#include "ps2.h"
#include "../acpi/apic.h"
#include "../acpi/apic.h"
#include <io.h>
#include <stdio.h>

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_REG  0x64
#define PS2_COMMAND_REG 0x64

void ps2_init(){
    outb(PS2_COMMAND_REG, 0xAE);
}


const char ascii_table[] = {
        0 ,  0 , '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=',  0 ,  0 ,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
        0 ,  0 , 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'','`',  0 , '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/',  0 , '*',
        0 , ' '
};

char ps2_translate_ascii(uint8_t scancode){
    if(scancode > 0x58){
        printf("scancode not char{n}");
        return 0;
    }

    if(ascii_table[scancode] == 0){
        printf("scancode is 0{n}");
        return 0;
    }
    printf("ascii_table[0x{x}]: 0x{xn}", scancode, ascii_table[scancode]);
    return ascii_table[scancode];
} 

void ps2_interrupt(){
    printf("scancode: {xn}", inb(PS2_DATA_PORT));
/*     char character = ps2_translate_ascii(scancode);
    if(character == NULL){
        printf("null character{n}");
        apic_eoi();
        return;
    }
    printf("character: {cn}", character); */
    apic_eoi(); 

}