#include "idt.h"
#include <stdbool.h>

__attribute__((aligned(0x10))) static idtentry idt[256];     // our IDTs
idt_r idtpr;            // the idtr ptr


void idt_set_gate(unsigned char num, unsigned int base, unsigned char type){

    idt[num].offset1        = (base & 0xFFFF);      // first 16 bits
    idt[num].segselector    = 0x08;                 // kernel offset in GDT
    idt[num].reserved       = 0;                    // 0
    idt[num].type           = type;                 // flags
    idt[num].offset2        = (base >> 16) & 0xFFFF;// last 16 bits
}


void idt_load(void){
    __asm__ volatile ("lidt %0" : : "m"(idtpr));    // load the new IDT
    __asm__ volatile ("sti");                       // set the interrupt flag
}



void idt_init(void){
    idtpr.limit = (sizeof(idtentry) * 256) -1;      // setup IDT pointer, 256 = max descriptors
    idtpr.base = (uintptr_t)&idt[0];

    isrs_init();

    idt_load();
}





