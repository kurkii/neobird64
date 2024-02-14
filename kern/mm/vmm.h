#include <stdint.h>


typedef struct{
    uint64_t *pml4addr;
} page_map;

#define PTE_BIT_PRESENT (1ul << 0ul)
#define PTE_BIT_READ_WRITE (1ul << 1ul)
#define PTE_BIT_ACCESS_ALL (1ul << 2ul)
#define PTE_BIT_WRITE_THROUGH_CACHING (1ul << 3ul)
#define PTE_BIT_DISABLE_CACHING (1ul << 4ul)
#define PTE_BIT_PDE_OR_PTE_ACCESSED (1ul << 5ul)
#define PTE_BIT_DIRTY (1ul << 6ul)
#define PTE_BIT_PAT_SUPPORTED (1ul << 7ul)
#define PTE_BIT_GLOBAL (1ul << 8ul)
#define PTE_BIT_EXECUTE_DISABLE (1ul << 63ul)

void vmm_map_page(page_map *pgmp, uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags);
void vmm_init();
/*
 * PML4 (Page Map Level 4):
 *  - level 4
 *  - bits 39 - 47
 *  - entries point to PDPTs
 *  - address stored in cr3
 * 
 * PDPT (Page Directory Pointer Table):
 *  - level 3
 *  - bits 30 - 38
 *  - entries points to PDs
 * 
 * PD (Page Directories):
 *  - level 2
 *  - bits 21 - 29
 *  - entries points to PTs
 * 
 * PT (Page Table):
 *  - level 1
 *  - bits 12 - 20
 *  - entries refer to pages
 * 
 * Individual Page:
 *  - level 0
 *  - bits 0 - 11
*/