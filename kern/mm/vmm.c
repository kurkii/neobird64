#include "vmm.h"
#include "pmm.h"
#include <log.h>
#include <limine.h>
#include <string.h>
#include <macros.h>
#include <stdio.h>

/* Thanks to MunkOS & Lyre for reference */

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

void vmm_set_ctx(page_map *page_map){

    __asm__ volatile (
        "movq %0, %%cr3\n"
        : : "r" ((uint64_t *)((uint64_t)(page_map->pml4addr) - hhdmoffset)) : "memory"
    );

}


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

    
    kernel_page_map.pml4addr = (uint64_t*)((uint64_t)kernel_page_map.pml4addr + hhdmoffset);

    printf("vmm: address of page table: 0x{xn}", (uint64_t)kernel_page_map.pml4addr-hhdmoffset);

    memset(kernel_page_map.pml4addr, 0, PAGE_SIZE);

    // map kernel, stolen
    extern linker_symbol_ptr text_start_addr, text_end_addr,
        rodata_start_addr, rodata_end_addr,
        data_start_addr, data_end_addr;

    uint64_t text_start = ALIGN_DOWN((uint64_t)text_start_addr, PAGE_SIZE),
        rodata_start = ALIGN_DOWN((uint64_t)rodata_start_addr, PAGE_SIZE),
        data_start = ALIGN_DOWN((uint64_t)data_start_addr, PAGE_SIZE),
        text_end = ALIGN_UP((uint64_t)text_end_addr, PAGE_SIZE),
        rodata_end = ALIGN_UP((uint64_t)rodata_end_addr, PAGE_SIZE),
        data_end = ALIGN_UP((uint64_t)data_end_addr, PAGE_SIZE); 


    // map pmm bitmap
    extern uint64_t pmm_block_count;
    extern uint64_t pmm_usable_addr;
    for(uint64_t i = 0; i < pmm_block_count/8 ;i+=4096){
        vmm_map_page(&kernel_page_map, pmm_usable_addr+hhdmoffset+i, pmm_usable_addr+i, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE);
    }
    // map usable entries, framebuffer and bootloader reclaimable shit
    extern struct limine_memmap_response *memmap;
    for(uint64_t i = 0; i < memmap->entry_count; i++){
        if(memmap->entries[i]->type == LIMINE_MEMMAP_USABLE){
            for(uint64_t j = 0; j < memmap->entries[i]->length; j+=PAGE_SIZE){
                vmm_map_page(&kernel_page_map, memmap->entries[i]->base+j+hhdmoffset, memmap->entries[i]->base+j, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE);
            }
        }
        if(memmap->entries[i]->type == LIMINE_MEMMAP_FRAMEBUFFER){
            for(uint64_t j = 0; j < memmap->entries[i]->length; j+=PAGE_SIZE){
                vmm_map_page(&kernel_page_map, memmap->entries[i]->base+j+hhdmoffset, memmap->entries[i]->base+j, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE | PTE_BIT_EXECUTE_DISABLE);
            }
        }
        if(memmap->entries[i]->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE){
            for(uint64_t j = 0; j < memmap->entries[i]->length; j+=PAGE_SIZE){
                vmm_map_page(&kernel_page_map, memmap->entries[i]->base+j+hhdmoffset, memmap->entries[i]->base+j, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE);
            }
        }

    }

    for (uintptr_t text_addr = text_start; text_addr < text_end; text_addr += PAGE_SIZE) {
        uintptr_t phys = text_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(&kernel_page_map, text_addr, phys, PTE_BIT_PRESENT);
    }

    for (uintptr_t rodata_addr = rodata_start; rodata_addr < rodata_end; rodata_addr += PAGE_SIZE) {
        uintptr_t phys = rodata_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(&kernel_page_map, rodata_addr, phys, PTE_BIT_PRESENT | PTE_BIT_EXECUTE_DISABLE);
    }

    for (uintptr_t data_addr = data_start; data_addr < data_end; data_addr += PAGE_SIZE) {
        uintptr_t phys = data_addr - kernel_address->virtual_base + kernel_address->physical_base;
        vmm_map_page(&kernel_page_map, data_addr, phys, PTE_BIT_PRESENT | PTE_BIT_READ_WRITE | PTE_BIT_EXECUTE_DISABLE);
    }

    // map the lapics
    extern uint32_t *lapic_address;
    vmm_map_page(&kernel_page_map, ALIGN_DOWN(((uint32_t)lapic_address+hhdmoffset), PAGE_SIZE), ALIGN_DOWN((uint32_t)lapic_address, PAGE_SIZE), PTE_BIT_PRESENT | PTE_BIT_READ_WRITE);

    vmm_set_ctx(&kernel_page_map);

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
    if(new_page == NULL){
        log_panic("PMM failed to allocate block for the vmm");
    }
    
    memset((uint64_t*)((uint64_t)new_page+hhdmoffset), 0, PAGE_SIZE);
    page_map_ptr[indx] = (uint64_t)new_page | PTE_BIT_PRESENT | PTE_BIT_READ_WRITE |  PTE_BIT_ACCESS_ALL;

    return (uint64_t*)((uint64_t)new_page + hhdmoffset);
}

static uint64_t *get_pt(uint64_t *page_map, uint64_t virtual_addr){
    uint64_t pmd_index = (virtual_addr >> 21) & 0x1ff;
    uint64_t pud_index = (virtual_addr >> 30) & 0x1ff;
    uint64_t pgd_index = (virtual_addr >> 39) & 0x1ff;
    uint64_t *pgd = NULL, *pud = NULL, *pmd = NULL, *pt = NULL;
    pgd = get_lower_table(page_map, pgd_index);
    if(!pgd){
        return NULL;
    }
    pud = get_lower_table(pgd, pud_index);
    if(!pgd){
        return NULL;
    }
    pt = get_lower_table(pud, pmd_index);
    if(!pgd){
        return NULL;
    }
    return pt;
}

void vmm_map_page(page_map *page_map, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags){
    uint64_t pt_index = (virtual_addr >> 12) & 0x1ff;
    uint64_t *pt = get_pt(page_map->pml4addr, virtual_addr);
    if(pt == NULL){
        log_panic("o shit");
    }
    pt[pt_index] = physical_addr | flags;

    asm volatile(
        "movq %%cr3, %%rax\n\
	    movq %%rax, %%cr3\n"
        : : : "rax"
   );
}

void vmm_free_page(page_map page_map, uint64_t virtual_addr){
    uint16_t pt_index = (virtual_addr >> 12) & 0x1ff;
    uint64_t *pt = get_pt(page_map.pml4addr, virtual_addr);
    pmm_free_block(pt[pt_index] & 0x000ffffffffff000);
    pt[pt_index] = 0;
    asm volatile(
        "movq %%cr3, %%rax\n\
	    movq %%rax, %%cr3\n"
        : : : "rax"
   );
}


