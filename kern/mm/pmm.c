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

uint64_t memmap_entry_count = 0;        // count of entries in limines memmap
uint64_t usable_addr        = 0;        // first usable address
uint64_t usable_top         = 0;        // top of the first usable address
uint64_t free_blocks        = 0;        // no. of free blocks
uint64_t block_count        = 0;        // no. of blocks
uint64_t mem_size           = 0;        // size of memory in bytes
uint8_t *bitmap             = 0;        // we are doing a bitmap allocator

struct limine_memmap_response *memmap;
extern uint64_t hhdmoffset;

void mmap_set(int bit){
    bitmap[bit/8] |= (1 << (bit%8));
}

bool mmap_test(int bit){
    return bitmap[bit/8] &  (1 << (bit%8));
}

void mmap_unset(int bit){
    bitmap[bit / 8] &= ~ (1 << (bit % 8));
}

uint64_t addr2block(uint64_t addr){
    return addr / BLOCK_SIZE;
}

uint64_t block2addr(uint64_t block){
    return block * BLOCK_SIZE;
}

int find_free_block(void){
	for (uint64_t i = 0; i < block_count / 8; i++)      // find first free block
		if (bitmap[i] != 0xff)                          // if it isnt all set
			for (int j = 0; j < 8; j++) {		        // loop through the uint8
				int bit = 1 << j;                       // move 1 to specific bit
				if (!(bitmap[i] & bit) )                // compare bit and bitmap bit
					return i*8+j;                       // i = bit, so bit*8 = byte in memory +j
			}
 
	return -1;
}

uint64_t alloc_block(){
    int block = find_free_block();
    mmap_set(block);
    return block * BLOCK_SIZE;
}

void dealloc_block(uint64_t block){
    mmap_unset(block);
}

void pmm_init(){

    if(memmap_request.response == NULL){
        log_panic("Memory map not recieved, halting");
    }
    
    memmap = memmap_request.response;
    struct limine_memmap_entry **entries = memmap->entries;
    memmap_entry_count = memmap->entry_count;

    for(int i = 0; i < memmap_entry_count; i++){  // fetch memory size
        switch(entries[i]->type){
            case LIMINE_MEMMAP_USABLE:
                mem_size += entries[i]->length;
                break;
        }
    }

    int usable_found = 0;
    for(int i = 0; i < memmap_entry_count; i++){    // parse memmap
        switch (entries[i]->type) {
            case LIMINE_MEMMAP_USABLE:
                if(usable_found == 0){
                    usable_addr = entries[i]->base;
                    usable_top = usable_addr + entries[i]->length;
                    usable_found = 1;
                }
                printf("pmm: usable: 0x{x}, length: 0x{xn}", entries[i]->base, entries[i]->length);
                
                break;
            default:
                printf("pmm: type={d}, base=0x{x}, length=0x{xn}", entries[i]->type, entries[i]->base, entries[i]->length);
                break;
        };
    }

    if(usable_found == 0){
        printf("pmm: no suitable memory to place the bitmap{n}");
        log_panic("Not enough memory, halting.");
    }

    free_blocks = (usable_top / BLOCK_SIZE) * 8;
    block_count = mem_size/BLOCK_SIZE;

    printf("pmm: memsize: {d}MiB{n}", mem_size/1048576);
    printf("pmm: block count: {d}{n}", (block_count));
    printf("pmm: size of bitmap: {d}KiB{n}", (block_count/8)/1024);

    printf("pmm: placing bitmap at 0x{xn}", usable_addr);
    bitmap = (uint8_t*)(usable_addr+hhdmoffset);
    memset(bitmap, 0xFF, block_count/8);    // set the entire bitmap as used

    for(uint64_t i = 0; i < memmap_entry_count; i++){   // set usable blocks in the bitmap
        switch (entries[i]->type) {
            case (LIMINE_MEMMAP_USABLE):
                for(uint64_t j = 0; j < entries[i]->length; j += BLOCK_SIZE){
                    mmap_unset(addr2block((entries[i]->base+j)));
                }
        }
    }

    for(int i = 0; i < block_count/8; i++){     // mark the space used by the bitmap as used
        mmap_set(addr2block(usable_addr)+i);
    }

}

uint64_t fetch_mem(){
    return mem_size;
}