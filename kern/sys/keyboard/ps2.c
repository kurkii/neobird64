#include "ps2.h"
#include "../acpi/apic.h"
#include "../acpi/apic.h"
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <video.h>

#define PS2_DATA_PORT       0x60
#define PS2_STATUS_REG      0x64
#define PS2_COMMAND_REG     0x64

#define NORMAL_STATE        0x00
#define SHIFT_STATE         0x01
#define CAPS_STATE          0x02

#define LEFT_SHIFT_PRESSED  0x2A
#define LEFT_SHIFT_RELEASED 0xAA

#define CAPS_LOCK_PRESSED   0x3A
#define CAPS_LOCK_RELEASED  0xBA

#define BACKSPACE_PRESSED   0x0E
#define BACKSPACE_RELEASED  0x8E

int keyboard_state = NORMAL_STATE;

void ps2_init(){
    outb(PS2_COMMAND_REG, 0xAD);
    inb(PS2_STATUS_REG);
    outb(PS2_COMMAND_REG, 0xA7);
    printf("ps2: assuming US QWERTY layout, scan code 1. other layouts not supported{n}");
    uint8_t j = 1;
    while(j != 0){
        inb(PS2_DATA_PORT);
        j = (inb(PS2_STATUS_REG)>>1) & 1;

    }
    outb(PS2_COMMAND_REG, 0xAE);
    
}


const char ascii_table[] = {                // thanks poncho
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

char ps2_translate2ascii(uint16_t scancode){
    if(scancode > 0x58){
        return 0;
    } 

    if(scancode == LEFT_SHIFT_PRESSED && keyboard_state != CAPS_STATE){
        keyboard_state = SHIFT_STATE;
        return 0;
    }

    if(scancode == CAPS_LOCK_PRESSED && keyboard_state == NORMAL_STATE){
        keyboard_state = CAPS_STATE;
        return 0;
    }else if(scancode == CAPS_LOCK_PRESSED && keyboard_state == SHIFT_STATE){
        keyboard_state = CAPS_STATE;
        return 0;
    }
    if(ascii_table[scancode] == 0){
        return 0;
    }
    
    if(keyboard_state == SHIFT_STATE || keyboard_state == CAPS_STATE){
        return toupper(ascii_table[scancode]);
    }else{
        return ascii_table[scancode];
    }
}


void ps2_handler(){
    uint16_t scancode = inb(PS2_DATA_PORT);

    if (scancode == LEFT_SHIFT_RELEASED && keyboard_state != CAPS_STATE) { 
        keyboard_state = NORMAL_STATE;
    }

    if(keyboard_state == CAPS_STATE && scancode == CAPS_LOCK_PRESSED){
        keyboard_state = NORMAL_STATE;
        apic_eoi();
        return;
    }
    
    char character = ps2_translate2ascii(scancode);
    if(character == NULL){
        apic_eoi();
        return;
    }

    printf("{c}", character);

    apic_eoi(); 

}