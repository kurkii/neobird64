#include "../include/limine.h"
#include <stdio.h>
#include <log.h>
#include <stdbool.h>
#include <string.h>
#include <video.h>
#include "pmm.h"


#define BLOCK_SIZE      4096      // 4kib block sizes, cause of paging or whatever the fuck
#define BLOCKS_PER_BYTE 8
#define BITMAP_START    0xAE5

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




void mmap_set (int bit) {
    bitmap[bit/8] |= (1 << (bit%8));
}

bool mmap_test (int bit) {
    return bitmap[bit/8] &  (1 << (bit%8));
}

uint64_t get_block_count(){
    return mem_size;
}

uint64_t addr2block(uint64_t addr){
    return addr / BLOCK_SIZE;
}

uint64_t block2addr(uint64_t block){
    return block * BLOCK_SIZE;
}

uint64_t bit2block(uint64_t bit){
    return (bit/8) / BLOCK_SIZE;
}

uint64_t block2bit(uint64_t block){
    return block * 8;
}
int find_free_block(void){
	//! find the first free bit
	for (uint8_t i = 0; i < get_block_count() / 8; i++)
		if (bitmap[i] != 0xff)
			for (int j = 0; j < 8; j++) {		//! test each bit in the byte
				int bit = 1 << j;
				if (!(bitmap[i] & bit) )
					return i*8+j;
			}
 
	return -1;
}

void init_bitmap(){
    
    struct limine_memmap_entry **entries = memmap->entries;
    for(uint64_t i = 0; i < memmap_entry_count; i++){
        switch (entries[i]->type) {
            case !(LIMINE_MEMMAP_USABLE)||!(LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE):
                for(uint64_t j = 0; j < entries[i]->length; j += BLOCK_SIZE){
                    mmap_set(addr2block((entries[i]->base+j) / BLOCK_SIZE));
                }
        }
    }

}

void parse_memmap(struct limine_memmap_entry **entries){
    int usable_found = 0;
    for(int i = 0; i < memmap_entry_count; i++){
        switch (entries[i]->type) {
            case LIMINE_MEMMAP_USABLE:
                if(usable_found == 0 && entries[i]->length > get_block_count()/BLOCKS_PER_BYTE){
                    usable_addr = entries[i]->base;
                    usable_top = usable_addr + entries[i]->length;
                    usable_found = 1;
                }
                printf("pmm: usable: 0x{x}, length: 0x{xn}", entries[i]->base, entries[i]->length);
                mem_size += entries[i]->length;
                break;
            default:
                printf("pmm: type={d}, base=0x{x}, length=0x{xn}", entries[i]->type, entries[i]->base, entries[i]->length);
                break;
        }
    }

    if(usable_found == 0){
        printf("pmm: no suitable memory to place the bitmap{n}");
        log_panic("Not enough memory, halting.");
    }
}


void pmm_init(){

    if(memmap_request.response == NULL){
        log_panic("Memory map not recieved, halting");
    }
    
    memmap = memmap_request.response;
    struct limine_memmap_entry **entries = memmap->entries;
    memmap_entry_count = memmap->entry_count;

    
    parse_memmap(entries);

    free_blocks = (usable_top / BLOCK_SIZE) * 8;
    block_count = mem_size/8;
    printf("memsize: {d}MB{n}", mem_size/1000000);
    printf("block count: {d}KB{n}", (block_count)/1000);

    printf("pmm: placing bitmap at 0x{xn}", usable_addr);
    bitmap = (uint64_t*)(usable_addr+hhdmoffset);

    
    //memset(bitmap, 0x90, );
    init_bitmap();

    printf("{dn}", find_free_block());
    printf("0x{xn}", find_free_block());




}