#include "apic.h"
#include "acpi.h"
#include <stdio.h>
#include <stdint.h>
#include <limine.h>
#include <io.h>
#include "../idt/idt.h"
#include "../pic/pic.h"

#define LAPIC_ID_REG        0x20            // lapic id register
#define LAPIC_VERSION_REG   0x30            // lapic version register
 
#define LAPIC_EOI_REG       0xB0            // end of interrupt register
#define LAPIC_SIVR_REG      0xF0            // spurious interrupt vector register

#define LAPIC_ICR_LOW       0x300           // low 32 bits of the ICR 
#define LAPIC_ICR_HIGH      0x310           // high 32 bits of the ICR (xAPIC only, 2xAPIC has a 64-bit MSR for the ICR)

#define LAPIC_LVT_LINT0     0x350
#define LAPIC_LVT_LINT1     0x360

#define LAPIC_TIMERDIV_REG  0x3E0
#define LAPIC_LVTTIMER_REG  0x320
#define LAPIC_INITCNT_REG   0x380
#define LAPIC_CURCNT_REG    0x390
#define LAPIC_PERIODIC      0x20000

#define LAPIC_EOI_COMMAND   0x0

#define IA32_APIC_BASE_MSR  0x1B

#define IOAPICID_REG        0x0
#define IOAPICVER_REG       0x1
#define IOAPICARB_REG       0x2
#define IOAPICREDTBL_REG(n) (0x10 + 2 * n)  // n being which redtbl register (0-24)


uint32_t *lapic_address;
uint64_t ioapic_address;

uint64_t apic_timer_ticks;

madt_ics_t ics_array[64];

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

void ioapic_redt_write(void *ioapicaddr, uint8_t reg, uint64_t value){
    uint64_t volatile *ioapicptr = (uint64_t volatile*) ioapicaddr;
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

void apic_eoi(){
    apic_write((void*)lapic_address, LAPIC_EOI_REG, LAPIC_EOI_COMMAND);
}

void apic_sleep(int ms){
    int curcnt = apic_timer_ticks;
    while (apic_timer_ticks - curcnt < ms) {
        ;
    }
}


void apic_timer(){
    apic_timer_ticks++;
    apic_eoi();
}

void ioapic_configure_redirentry(uint8_t entry, redirection_entry_t* redirentry_addr){
    redirection_entry_t *redirentry = redirentry_addr;
    redirentry->vector_num          = 32+entry;
}

void ioapic_init(){
/*     int gsi;
    uint32_t ioapic_aaddress;
    for(int i = 0; i < 64; i++){
        if(ics_array[i].type == 2){
            madt_iso_t *iso = (madt_iso_t*)ics_array[i].address;
            if(iso->source == 0){                                              // looks for legacy PIC IRQ 0
                printf("Found the timers GSI{n}");
                int gsi = iso->gsi;                                            // iso->gsi is the IOAPIC pin to which the apic timer is connected
            }
        }
        if(ics_array[i].type == 1){
            madt_ioapic_t *ioapic = (madt_ioapic_t*)ics_array[i].address;
            ioapic_aaddress = ioapic->ioapicaddr;
        }
    } */
}

uint64_t *get_madt_tables(madt_t *madt){
    int i = 0;
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
                    printf("Found CPU: {dn}", plapic->apicID);
                    ics_array[i].address = (uint64_t*)cur_ics;
                    ics_array[i].type = cur_ics->entry_type;
                    i++;
                    break;
                }
                break;
            case 1:
                
                printf("IOAPIC: {dn}", cur_ics->record_length);
                madt_ioapic_t *ioapic = (madt_ioapic_t*)cur_ics;
                
                ics_array[i].address = (uint64_t*)cur_ics;
                ics_array[i].type = cur_ics->entry_type;
                i++;

                ioapic_address = ioapic->ioapicaddr;
                printf("IOAPIC ID: {dn}", ioapic->ioapicID);

                break;
            case 2:
                ;
                ics_array[i].address = (uint64_t*)cur_ics;
                ics_array[i].type = cur_ics->entry_type;
                i++;
                madt_iso_t *iso = (madt_iso_t*)cur_ics;
                printf("ISO:{dn}", i);
                printf("bus:{dn}", iso->bus);
                printf("source:{dn}", iso->source);
                printf("gsi:{dnn}", iso->gsi);
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

void calibrate_timer(madt_t *madt){
    lapic_address = (uint32_t*)madt->lapicaddr;
    apic_write(lapic_address, LAPIC_TIMERDIV_REG, 0x3);         // set divisor 16
    apic_write(lapic_address, LAPIC_INITCNT_REG, 0xFFFFFFFF);   // set timer counter
    pmt_delay(10);

    apic_write(lapic_address, LAPIC_TIMERDIV_REG, 0x10000);     // 0x10000 = masked, sdm
    uint32_t calibration = 0xffffffff - apic_read(lapic_address, LAPIC_CURCNT_REG);
    apic_write(lapic_address, LAPIC_LVTTIMER_REG, 172 | LAPIC_PERIODIC);
    apic_write(lapic_address, LAPIC_TIMERDIV_REG, 0x3);         // 16
    apic_write(lapic_address, LAPIC_INITCNT_REG, calibration);

}

void init_apic(madt_t *madt, uint64_t hhdmoffset){ 
    asm("cli");
    // fetch madt tables
    get_madt_tables(madt);

    // initialize LAPIC
    printf("acpi: MADT tables listed through{n}");
    printf("acpi: writing to SIV register{n}");
    uint64_t lapic_address = madt->lapicaddr;
    uint32_t spurious_reg = apic_read((void*)madt->lapicaddr, LAPIC_SIVR_REG);
    
    apic_write((void*)madt->lapicaddr, LAPIC_SIVR_REG, spurious_reg | (0x100)); // start recieving ints

    // initalize timer
    //idt_set_gate(32, apic_timer, 0x8e);
    calibrate_timer(madt);
    
    // initalize IOAPIC
    //ioapic_init();
    asm("sti");
}