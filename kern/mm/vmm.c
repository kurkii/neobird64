#include "vmm.h"
#include "pmm.h"
#include <log.h>
#include <limine.h>
#include <string.h>
#include <macros.h>
#include <stdio.h>

#define PAGE_SIZE 4096

struct limine_kernel_address_request kernel_addr_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

struct limine_kernel_address_response *kernel_address;

page_map kernel_page_map = {0x0};

uint64_t hhdmoffset;

void vmm_init(){
    hhdmoffset = hhdm_request.response->offset;
    if(!hhdmoffset){
        log_panic("HHDM offset not recieved");
    }
    struct limine_kernel_address_response *kernel_address = kernel_addr_request.response;
    if(!kernel_address){
        log_panic("Kernel address not recieved");
    }
    /* the pml4 table is 4096 bytes wide, the same size as a pmm block, so the
    entire table can fit in one block, for which were allocating here */
    kernel_page_map.pml4addr = pmm_alloc_block(); 
    if(kernel_page_map.pml4addr == NULL){
        log_panic("Allocating block for page map failed");
    }

    printf("0x{xn}", kernel_page_map.pml4addr);
    
    kernel_page_map.pml4addr += hhdmoffset;

    printf("0x{xn}", kernel_page_map.pml4addr);

    memset((void*)kernel_page_map.pml4addr, 0, PAGE_SIZE);
    printf("YAY");

    extern linker_symbol_ptr text_start_addr, text_end_addr,
        rodata_start_addr, rodata_end_addr,
        data_start_addr, data_end_addr;

    uintptr_t text_start = ALIGN_DOWN((uintptr_t)text_start_addr, PAGE_SIZE),
        rodata_start = ALIGN_DOWN((uintptr_t)rodata_start_addr, PAGE_SIZE),
        data_start = ALIGN_DOWN((uintptr_t)data_start_addr, PAGE_SIZE),
        text_end = ALIGN_UP((uintptr_t)text_end_addr, PAGE_SIZE),
        rodata_end = ALIGN_UP((uintptr_t)rodata_end_addr, PAGE_SIZE),
        data_end = ALIGN_UP((uintptr_t)data_end_addr, PAGE_SIZE); 
    for (uintptr_t text_addr = text_start; text_addr < text_end; text_addr += PAGE_SIZE) {
        uintptr_t phys = text_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(kernel_page_map, text_addr, phys, PTE_BIT_PRESENT);
    }

    for (uintptr_t rodata_addr = rodata_start; rodata_addr < rodata_end; rodata_addr += PAGE_SIZE) {
        uintptr_t phys = rodata_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(kernel_page_map, rodata_addr, phys, PTE_BIT_PRESENT | PTE_BIT_EXECUTE_DISABLE);
    }

    for (uintptr_t data_addr = data_start; data_addr < data_end; data_addr += PAGE_SIZE) {
        uintptr_t phys = data_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(kernel_page_map, data_addr, phys, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE | PTE_BIT_EXECUTE_DISABLE);
    }
    printf("yo");
    asm volatile(
        "movq %%cr3, %%rax\n\
	    movq %%rax, %%cr3\n"
        : : : "rax"
   );
    
}

uint64_t *get_lower_table(uint64_t *page_map_ptr, uint64_t indx){
    if((page_map_ptr[indx] & PTE_BIT_PRESENT) != 0){
        return (uint64_t *)((page_map_ptr[indx] & 0x000ffffffffff000) + hhdmoffset);
    }    

    uint64_t *new_page = pmm_alloc_block(); 
    page_map_ptr[indx] = (uint64_t)new_page | PTE_BIT_PRESENT | PTE_BIT_READ_WRITE |  PTE_BIT_ACCESS_ALL;

    return new_page + hhdmoffset;
}

uint64_t *get_pt(uint64_t *page_map, uint64_t virtual_addr){
    uint16_t pt_index = (virtual_addr >> 12) & 0x1ff;
    uint16_t pmd_index = (virtual_addr >> 21) & 0x1ff;
    uint16_t pud_index = (virtual_addr >> 30) & 0x1ff;
    uint16_t pgd_index = (virtual_addr >> 39) & 0x1ff;

    uint64_t *pgd = NULL, *pud = NULL, *pmd = NULL, *pt = NULL;

    pgd = get_lower_table(page_map, pgd_index);
    if(!pgd){
        return NULL;
    }
    pud = get_lower_table(pgd, pud_index);
    if(!pud){
        return NULL;
    }
    pmd = get_lower_table(pud, pmd_index);
    if(!pmd){
        return NULL;
    }
    pt = get_lower_table(pmd, pt_index);
    if(!pt){
        return NULL;
    }
    return pt;
}

void vmm_map_page(page_map page_map, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags){
    uint16_t pt_index = (virtual_addr >> 12) & 0x1ff;
    uint64_t *pt = get_pt(kernel_page_map.pml4addr, virtual_addr);
    pt[pt_index] = physical_addr | flags;

    asm volatile(
        "movq %%cr3, %%rax\n\
	    movq %%rax, %%cr3\n"
        : : : "rax"
   );


}
