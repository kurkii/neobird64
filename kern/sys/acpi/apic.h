#pragma once

#include "acpi.h"

struct redirection_entry{
    uint8_t vector_num;             // 0x10 to 0xFE
    uint8_t deliverymode : 3;       // 0 normal, 1 low prio., 2 sys manag. int, 4 nmi, 5 init, 7 external
    uint8_t destinationmode : 1;    // 0 physical, 1 logical
    uint8_t busy : 1;
    uint8_t polarity : 1;           // 0 high, 1 low
    uint8_t LTIstatus : 1;          // 1 int recieved, 0 EOI
    uint8_t triggerMode : 1;        // 0 edge sens., 1 level sens.
    uint8_t intmask : 1;            // interrupt mask
    uint64_t reserved : 39;         // reserved
    uint8_t destination;            // low. 4 bits contain APIC id
};

void init_apic(madt_t *madt, uint64_t hhdmoffset);
void apic_eoi(void);
/* int apic_read(void *ioapicaddr, int reg);
void apic_write(void *ioapicaddr, int reg, int value); */