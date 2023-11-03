#include "apic.h"
#include "acpi.h"
#include <stdio.h>
#include <stdint.h>
#include <limine.h>

static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .flags = 0,
};



int apic_read(void *ioapicaddr, int reg){
    uint32_t volatile *ioapicptr = (uint32_t volatile*) ioapicaddr;
    ioapicptr[0] = (reg & 0xFF);
    return ioapicptr[4];
}

void apic_write(void *ioapicaddr, int reg, int value){
    uint32_t volatile *ioapicptr = (uint32_t volatile*) ioapicaddr;
    ioapicptr[0] = (reg & 0xFF);
    ioapicptr[4] = value;
}

/* void locate_lapics(madt_t *madt){
    madt_record_t *header;

    for(int i = 0; i < 32; i++){
        header = &madt->structs[i];
        if(header->entry_type == 2){
            printf("found 2{n}");
        }
        *header = *(header + header->record_length);
    }
} */


void init_apic(madt_t *madt, uint64_t hhdmoffset){  

    uint32_t *lapic_addr;
    uint32_t *ioapic_addr;

    uint64_t cpucount = smp_request.response->cpu_count;
    lapic_addr = (uint32_t*)(madt->lapicaddr+hhdmoffset);


     
}