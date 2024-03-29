#include "gdt.h"
#include <string.h>

#define GDTSEGMENTS 7 // 6 + 1 - TSS takes up two segments

static gdtdesc gdt[GDTSEGMENTS];
gdtptr gdtr;

tssdesc tss;

void gdt_set_entry(int num, unsigned long long base, unsigned long long limit, unsigned char access, unsigned char granularity){
    // descriptor base access
    gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	// descriptor limits
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	// granularity and access flag
	gdt[num].granularity |= (granularity & 0xF) << 4;
	gdt[num].access = access;
}

void gdt_init(){

    gdtr.limit_low = (sizeof(gdtdesc) * GDTSEGMENTS) - 1;
    gdtr.base = (uint64_t)&gdt;

    memset(&tss, 0, sizeof(tss));
    // entry number, base, limit, access, granularity

    // ENTRY 0 - NULL DESCRIPTOR            - 0x00
    gdt_set_entry(0, 0, 0, 0, 0);
    // ENTRY 1 - KERNEL MODE CODE SEGMENT   - 0x08
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA);
    // ENTRY 2 - KERNEL MODE DATA SEGMENT   - 0x10
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xA);
    // ENTRY 3 - USER MODE CODE SEGMENT     - 0x18
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA);
    // ENTRY 4 - USER MODE DATA SEGMENT     - 0x20
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xA);
    // ENTRY 5 - TSS                        - 0x28 (note: TSS takes up 2 GDT entries)
    gdt_set_entry(5, (uint64_t)&tss, sizeof(tss), 0x89, 0x0);
    
    s_setgdt(gdtr.limit_low, gdtr.base);
    
    s_settss();

}
