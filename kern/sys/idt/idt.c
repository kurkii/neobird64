#include "idt.h"
#include "../acpi/apic.h"
#include "../keyboard/ps2.h"
#include <stdio.h>
#include <stdbool.h>

__attribute__((aligned(0x10))) static idtentry idt[256];     // our IDTs
idtrr idtpr;            // the idtr ptr


void idt_set_gate(unsigned char vector, void *isr, uint8_t flags){

    idtentry* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}


void idt_load(void){
    __asm__ volatile ("lidt %0" : : "m"(idtpr));    // load the new IDT
    __asm__ volatile ("sti");                       // set the interrupt flag
}


extern void s_isr0();
extern void s_isr1();
extern void s_isr2();
extern void s_isr3();
extern void s_isr4();
extern void s_isr5();
extern void s_isr6();
extern void s_isr7();
extern void s_isr8();
extern void s_isr9();
extern void s_isr10();
extern void s_isr11();
extern void s_isr12();
extern void s_isr13();
extern void s_isr14();
extern void s_isr15();
extern void s_isr16();
extern void s_isr17();
extern void s_isr18();
extern void s_isr19();
extern void s_isr20();
extern void s_isr21();
extern void s_isr22();
extern void s_isr23();
extern void s_isr24();
extern void s_isr25();
extern void s_isr26();
extern void s_isr27();
extern void s_isr28();
extern void s_isr29();
extern void s_isr30();
extern void s_isr31();

extern void s_isr33();      // PS/2 keyboard

extern void s_isr172();     // apic timer

void idt_init(void){
    idtpr.base = (uintptr_t)&idt[0];
    idtpr.limit = (uint16_t)sizeof(idtentry) * 256 - 1;
 
    idt_set_gate(0, s_isr0, 0x8E);
    idt_set_gate(1, s_isr1, 0x8E);
    idt_set_gate(2, s_isr2, 0x8E);
    idt_set_gate(3, s_isr3, 0x8E);
    idt_set_gate(4, s_isr4, 0x8E);
    idt_set_gate(5, s_isr5, 0x8E);
    idt_set_gate(6, s_isr6, 0x8E);
    idt_set_gate(7, s_isr7, 0x8E);
    idt_set_gate(8, s_isr8, 0x8E);
    idt_set_gate(9, s_isr9, 0x8E);
    idt_set_gate(10, s_isr10, 0x8E);
    idt_set_gate(11, s_isr11, 0x8E);
    idt_set_gate(12, s_isr12, 0x8E);
    idt_set_gate(13, s_isr13, 0x8E);
    idt_set_gate(14, s_isr14, 0x8E);
    idt_set_gate(15, s_isr15, 0x8E);
    idt_set_gate(16, s_isr16, 0x8E);
    idt_set_gate(17, s_isr17, 0x8E);
    idt_set_gate(18, s_isr18, 0x8E);
    idt_set_gate(19, s_isr19, 0x8E);
    idt_set_gate(20, s_isr20, 0x8E);
    idt_set_gate(21, s_isr21, 0x8E);
    idt_set_gate(22, s_isr22, 0x8E);
    idt_set_gate(23, s_isr23, 0x8E);
    idt_set_gate(24, s_isr24, 0x8E);
    idt_set_gate(25, s_isr25, 0x8E);
    idt_set_gate(26, s_isr26, 0x8E);
    idt_set_gate(27, s_isr27, 0x8E);
    idt_set_gate(28, s_isr28, 0x8E);
    idt_set_gate(29, s_isr29, 0x8E);
    idt_set_gate(30, s_isr30, 0x8E);
    idt_set_gate(31, s_isr31, 0x8E);
    idt_set_gate(33, s_isr33, 0x8E);
    idt_set_gate(172, s_isr172, 0x8E);

    idt_load();
}

char *exception_messages[] =
{
    "Division Error",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void exception_handler(struct int_frame *r){
    if(r->int_no < 32){
        printf("interrupt no. {d}: {snn}", r->int_no, exception_messages[r->int_no]);
        printf("rax {d} | rbx {d} | rcx {d} | rdx {dn}", r->rax, r->rbx, r->rcx, r->rdx);
        printf("rdi {d} | rsi 0x{x} | rbp 0x{xn}", r->rdi, r->rsi, r->rbp);
        printf("r8 {d} | r9 {d} | r10 {d} | r11 {d} | r12 {d} | r13 {d} | r14 {d} | r15 {dn}", r->r8, r->r9, r->r10, r->r11, r->r12, r->r13, r->r14, r->r15);
        printf("rip 0x{x} | cs {d} | ss {d} | rsp 0x{x} | rflags 0x{xn}", r->rip, r->cs, r->ss, r->rsp, r->rflags);
        __asm__ volatile ("cli; hlt"); // Completely hangs the computer
    }

    if(r->int_no == 0xAC){               // apic timer
        apic_timer();
    }

    if(r->int_no == 33){
        ps2_interrupt();
    }
}