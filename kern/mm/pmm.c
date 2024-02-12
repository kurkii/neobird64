#include <limine.h>
#include <stdio.h>
#include <log.h>
#include <stdbool.h>
#include <string.h>
#include <video.h>
#include "pmm.h"

#define BLOCK_SIZE      4096      // 4kib block sizes, cause of paging or whatever the fuck

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

uint64_t memmap_entry_count     = 0;        // count of entries in limines memmap
uint64_t pmm_usable_addr        = 0;        // first usable address
uint64_t pmm_usable_top         = 0;        // top of the first usable address
uint64_t pmm_free_blocks        = 0;        // no. of free blocks
uint64_t pmm_block_count        = 0;        // no. of blocks
uint64_t pmm_mem_size           = 0;        // size of memory in bytes
uint8_t *pmm_bitmap             = 0;        // we are doing a bitmap allocator

struct limine_memmap_response *memmap;
extern uint64_t hhdmoffset;

void mmap_set(int bit){
    pmm_bitmap[bit/8] |= (1 << (bit%8));
}

bool mmap_test(int bit){
    return pmm_bitmap[bit/8] &  (1 << (bit%8));
}

void mmap_unset(int bit){
    pmm_bitmap[bit / 8] &= ~ (1 << (bit % 8));
}

uint64_t addr2block(uint64_t addr){
    return addr / BLOCK_SIZE;
}

uint64_t block2addr(uint64_t block){
    return block * BLOCK_SIZE;
}

uint64_t find_free_block(void){
	for (uint64_t i = 0; i < pmm_block_count / 8; i++)      // find first free block
		if (pmm_bitmap[i] != 0xff)                          // if it isnt all set
			for (int j = 0; j < 8; j++) {		        // loop through the uint8
				int bit = 1 << j;                       // move 1 to specific bit
				if (!(pmm_bitmap[i] & bit) )                // compare bit and bitmap bit
					return i*8+j;                       // i = bit, so bit*8 = byte in memory +j
			}
 
	return -1;
}

uint64_t *pmm_alloc_block(){
    uint64_t block = find_free_block();
    if(block == -1){
        return NULL;
    }
    

    mmap_set(block);
    return (uint64_t*)(block * BLOCK_SIZE);
}

void pmm_free_block(uint64_t addr){
    mmap_unset(addr2block(addr));
}

void pmm_init(){

    if(memmap_request.response == NULL){
        log_panic("Memory map not recieved, halting");
    }
    
    memmap = memmap_request.response;
    struct limine_memmap_entry **entries = memmap->entries;
    memmap_entry_count = memmap->entry_count;

    for(uint64_t i = 0; i < memmap_entry_count; i++){  // fetch memory size
        switch(entries[i]->type){
            case LIMINE_MEMMAP_USABLE:
                pmm_mem_size += entries[i]->length;
                break;
        }
    }

    int usable_found = 0;
    for(uint64_t i = 0; i < memmap_entry_count; i++){    // parse memmap
        switch (entries[i]->type) {
            case LIMINE_MEMMAP_USABLE:
                if(usable_found == 0){
                    pmm_usable_addr = entries[i]->base;
                    pmm_usable_top = pmm_usable_addr + entries[i]->length;
                    usable_found = 1;
                }
                printf("pmm: usable: 0x{x}, length: 0x{xn}", entries[i]->base, entries[i]->length);
                
                break;
            default:
                printf("pmm: type={d}, base=0x{x}, length=0x{xn}", entries[i]->type, entries[i]->base, entries[i]->length);
                break;
        };
    }

    pmm_free_blocks = (pmm_usable_top / BLOCK_SIZE) * 8;
    pmm_block_count = pmm_mem_size/BLOCK_SIZE;

    printf("pmm: memsize: {d}MiB{n}", pmm_mem_size/1048576);
    printf("pmm: block count: {d}{n}", (pmm_block_count));
    printf("pmm: size of bitmap: {d}KiB{n}", (pmm_block_count/8)/1024);

    printf("pmm: placing bitmap at 0x{xn}", pmm_usable_addr);
    pmm_bitmap = (uint8_t*)(pmm_usable_addr+hhdmoffset);
    memset(pmm_bitmap, 0xFF, pmm_block_count/8);    // set the entire bitmap as used

    for(uint64_t i = 0; i < memmap_entry_count; i++){   // set usable blocks in the bitmap
        switch (entries[i]->type) {
            case (LIMINE_MEMMAP_USABLE):
                for(uint64_t j = 0; j < entries[i]->length; j += BLOCK_SIZE){
                    mmap_unset(addr2block((entries[i]->base+j)));
                }
        }
    }


    for(uint64_t i = 0; i < pmm_block_count/8; i++){     // mark the space used by the bitmap as used
        mmap_set(addr2block(pmm_usable_addr)+i);
    }

}

uint64_t pmm_fetch_mem(){
    return pmm_mem_size;
}