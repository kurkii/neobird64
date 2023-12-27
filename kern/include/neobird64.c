#include <stdint.h>
#include "../pmm/pmm.h"

uint64_t get_memory_size(void){
    return fetch_mem();
}