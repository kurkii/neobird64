#include "smp.h"
#include <stdint.h>
#include "../acpi/acpi.h"
#include "../acpi/apic.h"
#include "../../mm/pmm.h"
#include "../../mm/vmm.h"
#include "../gdt.h"
#include "../idt.h"
#include <limine.h>
#include <stdio.h>
#include <macros.h>
#include <string.h>
#include <log.h>

struct limine_smp_request smp_request = {
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
}; 

bool state_single_processor;

/* struct limine_smp_info smp_info {
    uint32_t processor_id;
    uint32_t lapic_id;
    uint64_t reserved;
    limine_goto_address goto_address;
    uint64_t extra_argument;
};  */

extern uint64_t hhdmoffset;
extern page_map kernel_page_map;

void ap_trampoline(struct limine_smp_info *smp_info){
    if(smp_info->lapic_id == 0){
        return;
    }
    asm("cli");
    gdt_init(); // beware, tss is the same for all cpus
    idt_load();
    vmm_set_ctx(&kernel_page_map);
    asm volatile(
        "movq %%cr3, %%rax\n\
	    movq %%rax, %%cr3\n"
        : : : "rax"
   ); 
    init_apic_ap(); // enables the lapic and lapic timer
    asm("sti");
    printf("smp: cpu {d} started {n}", smp_info->processor_id);
    for(;;){
        asm("hlt");
    }

}

void smp_init(){
    if(smp_request.response == NULL){
        log_panic("SMP request unfulfilled");
    }

    struct limine_smp_info **smp_info = smp_request.response->cpus;
    if(smp_request.response->cpu_count == 1){
        printf("smp: single core processor, skipping smp initalization{n}");
        state_single_processor = true;
        return;
    }
    printf("smp: cpu count: {dn}", smp_request.response->cpu_count);
    for(uint64_t i = 0; i < smp_request.response->cpu_count; i++){
        apic_sleep(2); // why does this fix triple fault on real hardware?
        printf("smp: starting cpu {dn}", i);
        smp_info[i]->goto_address = &ap_trampoline;
    }
    printf("smp: CPUs started{n}");
}
