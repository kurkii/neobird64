#include <stdint.h>
#include <stdbool.h>

#pragma once

void pmm_init();
uint64_t pmm_fetch_mem();
uint64_t *pmm_alloc_block();
void free_block(uint64_t block);
uint64_t addr2block(uint64_t addr);
bool mmap_test(int bit);