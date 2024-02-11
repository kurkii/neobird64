#include <stdint.h>
#include "../mm/pmm.h"

uint64_t get_memory_size(void){
    return pmm_fetch_mem();
}