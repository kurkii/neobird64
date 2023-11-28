#include <stdint.h>

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t	    ist;          // The IST in the TSS that the CPU will load into RSP; set to zero for now
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t    isr_high;     // The higher 32 bits of the ISR's address
	uint32_t    reserved;     // Set to zero
} __attribute__((packed)) idtentry;

typedef struct {
	uint16_t	limit;
	uint64_t	base;
} __attribute__((packed)) idtrr;

struct int_frame
{
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rdx, rcx, rbx, rax;
	uint64_t int_no, err;
	uint64_t rip, cs, ss, rsp, rflags;
};

void idt_set_gate(unsigned char vector, void *isr, uint8_t flags);
__attribute((noreturn))void exception_handler(struct int_frame *r);

void idt_init(void);