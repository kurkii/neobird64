#pragma once

#include "acpi.h"

typedef struct{           // https://web.archive.org/web/20161130153145/http://download.intel.com/design/chipsets/datashts/29056601.pdf pg. 11 
    uint8_t vector_num;             // 0x10 to 0xFE
    uint8_t delivery_mode : 3;      // 0 normal, 1 low prio., 2 sys manag. int, 4 nmi, 5 init, 7 external
    uint8_t destination_mode : 1;   // 0 physical, 1 logical
    uint8_t delivery_status : 1;    // 0 idle, 1 busy
    uint8_t intpol : 1;             // 0 high, 1 low - specifies polarity of interrupt
    uint8_t remote_irr : 1;         // 1 int recieved, 0 EOI
    uint8_t trigger_mode : 1;       // 0 edge sens., 1 level sens.
    uint8_t intmask : 1;            // interrupt mask
    uint64_t reserved : 39;         // reserved
    uint8_t destination;            // low. 4 bits contain APIC id
} __attribute((packed))redirection_entry_t;

typedef struct{
    uint64_t *address;
    uint8_t  type;
} __attribute((packed))madt_ics_t;

void init_apic(madt_t *madt, uint64_t hhdmoffset);
void apic_eoi(void);
void apic_timer();
void apic_sleep(int ms);
/* int apic_read(void *ioapicaddr, int reg);
void apic_write(void *ioapicaddr, int reg, int value); */