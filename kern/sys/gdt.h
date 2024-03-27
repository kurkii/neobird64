#include <stdint.h>
typedef struct gdtdesc {
    uint16_t limit_low;
    uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdtdesc;

typedef struct gdtptr {
    uint16_t limit_low;
    uint64_t base;
}__attribute__((packed)) gdtptr;

typedef struct tssdesc {
    uint32_t reserved1;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved2;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iopb;
} __attribute__((packed)) tssdesc;

void gdt_set_entry(int num, unsigned long long base, unsigned long long limit, unsigned char access, unsigned char granularity);
void gdt_init();
extern void s_setgdt(uint32_t limit, uint64_t base);
extern void s_flushgdt(void);
extern void s_settss(void);