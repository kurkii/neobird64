#include "apic.h"
#include "acpi.h"
#include <stdio.h>
#include <stdint.h>
#include <limine.h>
#include <io.h>

#define LAPIC_ID_REG        0x20        // lapic id register
#define LAPIC_VERSION_REG   0x30        // lapic version register
 
#define LAPIC_EOI_REG       0xB0        // end of interrupt register
#define LAPIC_SIVR_REG      0xF0        // spurious interrupt vector register

#define LAPIC_ICR_LOW       0x300       // low 32 bits of the ICR 
#define LAPIC_ICR_HIGH      0x310       // high 32 bits of the ICR (xAPIC only, 2xAPIC has a 64-bit MSR for the ICR)

#define LAPIC_TIMERDIV_REG  0x3E0
#define LAPIC_LVTTIMER_REG  0x320

#define LAPIC_EOI_COMMAND   0x0

#define IA32_APIC_BASE_MSR  0x1B




uint64_t lapic_address;
uint64_t ioapic_address;


static volatile struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0,
    .flags = 0,
};


int ioapic_read(void *ioapicaddr, uint8_t reg){
    uint32_t volatile *ioapicptr = (uint32_t volatile*) ioapicaddr;
    ioapicptr[0] = (reg & 0xFF);
    return ioapicptr[4];
}

void ioapic_write(void *ioapicaddr, uint8_t reg, uint32_t value){
    uint32_t volatile *ioapicptr = (uint32_t volatile*) ioapicaddr;
    ioapicptr[0] = (reg & 0xFF);
    ioapicptr[4] = value;
}

uint32_t apic_read(void* apic_base, uint32_t reg) {
    return *((volatile uint32_t*)(apic_base + reg));
}

void apic_write(void* apic_base, uint32_t reg, uint32_t data) {
    *((volatile uint32_t*)(apic_base + reg)) = data;
}

void apic_send_interrupt(void* apic_base, uint8_t apic, uint8_t vector) {
    apic_write(apic_base, LAPIC_ICR_HIGH, ((uint32_t)apic) << 24);
    apic_write(apic_base, LAPIC_ICR_LOW, vector);
}

void apic_timer(){
    printf("hi{n}");
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

uint64_t *get_madt_tables(madt_t *madt){
    madt_record_t *cur_ics = &madt->first_ics;
    int length = madt->header.length - sizeof(madt_t) + 2;
    while(length > 0){
        switch(cur_ics->entry_type){
            case 0:
                ;
                madt_plapic_t *plapic = (madt_plapic_t*)cur_ics;
                if(plapic->flags == 0){
                    break;
                }else{
                    break;
                }
                //printf("Type: {dn}", cur_ics->entry_type);
                break;
            case 1:
                
                //printf("IOAPIC: {dn}", cur_ics->record_length);
                
                //printf("Type: {dn}", cur_ics->entry_type);
                break;
            case 2:
                //printf("NMIs: {dn}", cur_ics->record_length);
                break;
        }
        //printf("length: {dn}", length);
        length -= cur_ics->record_length;
        cur_ics = (madt_record_t*)((void*)cur_ics+cur_ics->record_length);
    } 
}

uint64_t get_lapic_base(){
    return rdmsr(IA32_APIC_BASE_MSR);
}

void init_apic(madt_t *madt, uint64_t hhdmoffset){ 
    get_madt_tables(madt);
    printf("acpi: MADT tables listed through{n}");
    printf("acpi: writing to SIV register{n}");
    uint32_t apic_base;
    apic_base = madt->lapicaddr;
    printf("{x}", apic_base);
    uint32_t spurious_reg = apic_read((void*)apic_base, LAPIC_SIVR_REG);
    apic_write((void*)apic_base, LAPIC_SIVR_REG, spurious_reg | 0x1FF);
    printf("acpi: test interrupt{n}");
    apic_send_interrupt((void*)apic_base, 0, 32);   // ICR: sdm 11.6.1
    
}