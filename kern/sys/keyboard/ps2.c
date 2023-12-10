#include "ps2.h"
#include "../acpi/apic.h"
#include "../acpi/apic.h"
#include <io.h>
#include <stdio.h>
#include <video.h>

#define PS2_DATA_PORT   0x60
#define PS2_STATUS_REG  0x64
#define PS2_COMMAND_REG 0x64

#define NORMAL_STATE    0
#define SHIFT_STATE     1

int keyboard_state = NORMAL_STATE;

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

char ps2_translate_ascii(uint16_t scancode){
    if(scancode > 0x58){
        //printf("scancode not char{n}");
        return 0;
    }

    if(scancode == 0x2A){
        keyboard_state = SHIFT_STATE;
    }

    if(ascii_table[scancode] == 0){
        //printf("scancode is 0{n}");
        return 0;
    }
    
    if(keyboard_state == SHIFT_STATE){
        //printf(" shift ");
        return toupper(ascii_table[scancode]);
    }else{
        return ascii_table[scancode];
    }
}


void ps2_interrupt(){

    uint16_t scancode = inb(PS2_DATA_PORT);

    if (scancode == 0xAA) {
        keyboard_state = NORMAL_STATE;
    }
    
    char character = ps2_translate_ascii(scancode);
    if(character == NULL){
        apic_eoi();
        return;
    }
    printf("{c}", character);
    apic_eoi(); 

}