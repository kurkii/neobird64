#include "idt.h"
#include "irq.h"
#include "../../include/stdio.h"
#include "../../include/kernmsg.h"
#include "../io/io.h"

//  first 32 ISRs are reserved
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

void isrs_init(void){
    idt_set_gate(0, (unsigned long)s_isr0, 0x8E);
    idt_set_gate(1, (unsigned long)s_isr1, 0x8E);
    idt_set_gate(2, (unsigned long)s_isr2, 0x8E);
    idt_set_gate(3, (unsigned long)s_isr3, 0x8E);
    idt_set_gate(4, (unsigned long)s_isr4, 0x8E);
    idt_set_gate(5, (unsigned long)s_isr5, 0x8E);
    idt_set_gate(6, (unsigned long)s_isr6, 0x8E);
    idt_set_gate(7, (unsigned long)s_isr7, 0x8E);
    idt_set_gate(8, (unsigned long)s_isr8, 0x8E);
    idt_set_gate(9, (unsigned long)s_isr10, 0x8E);
    idt_set_gate(11, (unsigned long)s_isr11, 0x8E);
    idt_set_gate(12, (unsigned long)s_isr12, 0x8E);
    idt_set_gate(13, (unsigned long)s_isr13, 0x8E);
    idt_set_gate(14, (unsigned long)s_isr14, 0x8E);
    idt_set_gate(15, (unsigned long)s_isr15, 0x8E);
    idt_set_gate(16, (unsigned long)s_isr16, 0x8E);
    idt_set_gate(17, (unsigned long)s_isr17, 0x8E);    
    idt_set_gate(18, (unsigned long)s_isr18, 0x8E);
    idt_set_gate(19, (unsigned long)s_isr19, 0x8E);
    idt_set_gate(20, (unsigned long)s_isr20, 0x8E);
    idt_set_gate(21, (unsigned long)s_isr21, 0x8E);
    idt_set_gate(22, (unsigned long)s_isr22, 0x8E);
    idt_set_gate(23, (unsigned long)s_isr23, 0x8E);
    idt_set_gate(24, (unsigned long)s_isr24, 0x8E);
    idt_set_gate(25, (unsigned long)s_isr25, 0x8E);
    idt_set_gate(26, (unsigned long)s_isr26, 0x8E);
    idt_set_gate(27, (unsigned long)s_isr27, 0x8E);
    idt_set_gate(28, (unsigned long)s_isr28, 0x8E);
    idt_set_gate(29, (unsigned long)s_isr29, 0x8E);
    idt_set_gate(30, (unsigned long)s_isr30, 0x8E);
    idt_set_gate(31, (unsigned long)s_isr31, 0x8E);
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

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */

void exception_handler(struct regs *r) {
    
        kernel_msg('e', "Exception");
        panic(exception_messages[r->int_no]);
    
    for(;;);
}