#include "vmm.h"
#include "pmm.h"
#include <string.h>

#define PAGE_SIZE 4096

page_map kernel_page_map = {0}; // page map base addr

extern uint64_t hhdmoffset;

void init_vmm(){
    /* the pml4 table is 4096 bytes wide, the same size as a pmm block, so the
    entire table can fit in one block, for which were allocating here */
    kernel_page_map.pml4addr = pmm_alloc_block(); 
    if(kernel_page_map.pml4addr == NULL){
        // to do. find a way to check if result isnt good
    }

    kernel_page_map.pml4addr += hhdmoffset;
    memset((uint64_t*)kernel_page_map.pml4addr, 0, PAGE_SIZE); 
}

uint64_t *get_lower_table(uint64_t *page_map_ptr, uint64_t indx){
    if((page_map_ptr[indx] & PTE_BIT_PRESENT) != 0){
        return (uint64_t *)((page_map_ptr[indx] & 0x000ffffffffff000) + hhdmoffset);
    }    

    uint64_t new_page = pmm_alloc_block(); 
}

uint64_t *get_pt(uint64_t page_map, uint64_t virtual_addr){
    uint16_t pt_index = (virtual_addr >> 12) & 0x1ff;
    uint16_t pmd_index = (virtual_addr >> 21) & 0x1ff;
    uint16_t pud_index = (virtual_addr >> 30) & 0x1ff;
    uint16_t pgd_index = (virtual_addr >> 39) & 0x1ff;


}
