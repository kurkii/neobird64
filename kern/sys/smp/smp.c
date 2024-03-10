#include "smp.h"
#include "../acpi/acpi.h"
#include "../acpi/apic.h"
#include "../../mm/pmm.h"
#include "../../mm/vmm.h"
#include <limine.h>
#include <stdio.h>
#include <macros.h>
#include <string.h>

/* struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
}; */

/*
struct limine_smp_info {
    uint32_t processor_id;
    uint32_t lapic_id;
    uint64_t reserved;
    limine_goto_address goto_address;
    uint64_t extra_argument;
};  */
#define INIT_IPI            0x000C4500
#define STARTUP_IPI         0x000C4600
#define LAPIC_ICR_LOW       0x300           // low 32 bits of the ICR 
#define LAPIC_ICR_HIGH      0x310           // high 32 bits of the ICR (xAPIC only, 2xAPIC has a 64-bit MSR for the ICR)
#define PAGE_SIZE           0x1000
uint64_t ap_boot_addr;

extern uint64_t pmm_usable_addr;
extern uint64_t pmm_block_count;

void ap_boot(){
    for(;;){
        printf("hi");
    }
}

void smp_init(){
    extern uint32_t *lapic_address;
    uint32_t icr_low = 0;
    apic_write(lapic_address, LAPIC_ICR_LOW, icr_low | INIT_IPI); // send init command to all cpus except bp
    ap_boot_addr = ALIGN_UP(pmm_usable_addr + pmm_block_count/8, PAGE_SIZE); // find a place to put the trampoline code
    printf("0x{xn}", ap_boot_addr);
    extern page_map kernel_page_map;
    extern uint64_t hhdmoffset;
    vmm_map_page(&kernel_page_map, ap_boot_addr+hhdmoffset, ap_boot_addr, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE); // map the trampoline code to the kernels page map
    uint64_t ap_boot_vector = ap_boot_addr >> 12; // find the vector of the address
/*     void (*func_ptr)(void) = (void (*)(void))ap_boot;
    void (*ap_trampoline)(void) = (void (*)(void))ap_boot_addr+hhdmoffset; */
    memcpy((void*)(ap_boot_addr+hhdmoffset), ap_boot, PAGE_SIZE); // copy the trampoline code (function ap_boot) to the trampoline code location
    printf("0x{xn}", ap_boot_vector);
    printf("0x{xn}", STARTUP_IPI+ap_boot_vector);
    apic_write(lapic_address, LAPIC_ICR_LOW, STARTUP_IPI+ap_boot_vector); // send startup ipi twice, as per sdm
    apic_sleep(2);
    apic_write(lapic_address, LAPIC_ICR_LOW, STARTUP_IPI+ap_boot_vector);
    apic_sleep(2);
}