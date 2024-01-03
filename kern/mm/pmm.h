#include <stdint.h>

#pragma once

void pmm_init();
uint64_t fetch_mem();
uint64_t alloc_block();
void dealloc_block(uint64_t block);