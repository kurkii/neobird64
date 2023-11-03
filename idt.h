#include "stdint.h"
#ifndef IDT_H
#define IDT_H
typedef struct {
    uint16_t offset1;       // first 16 bits of offset
    uint16_t segselector;   // segment selector, points to code segment in GDT / kernel segment
    uint8_t reserved;       // 8bit unused zero region used for nefarious reasons by the CPU
    uint8_t type;           // stores the: 4 bit gate type - the type of gate this descriptor represents, the 2 bit DPL - defines which rings can access this interrupt and P - 1 bit which is always set to 1.
    uint16_t offset2;       // last 16 bits of offset
}  __attribute__((packed)) idtentry;

typedef struct{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_r;

extern void *isr_stub_table[];


void idt_set_gate(unsigned char num, unsigned int base, unsigned char type);
void idt_init(void);
void isrs_init(void);

struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

__attribute__((noreturn)) void exception_handler(struct regs *r);

#endif


