#include "vmm.h"
#include "pmm.h"
#include <string.h>

#define PAGE_SIZE 4096

page_map kernel_page_map = {0};

extern uint64_t hhdmoffset;

void init_vmm(){
    kernel_page_map.pml4addr = pmm_alloc_block();
    if(kernel_page_map.pml4addr == NULL){
        // to do. find a way to check if result isnt good
    }

    kernel_page_map.pml4addr += hhdmoffset;
    memset((uint64_t*)kernel_page_map.pml4addr, 0, PAGE_SIZE);
}
