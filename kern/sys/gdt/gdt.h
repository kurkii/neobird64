#include <stdint.h>
#define GDTSEGMENTS 5
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
    uint16_t reserved1;
    uint8_t rsp0;
    uint8_t rsp1;
    uint8_t rsp2;
    uint8_t reserved2;
    uint8_t ist1;
    uint8_t ist2;
    uint8_t ist3;
    uint8_t ist4;
    uint8_t ist5;
    uint8_t ist6;
    uint8_t ist7;
    uint8_t reserved3;
    uint16_t reserved4;
    uint16_t iopb;
} __attribute__((packed)) tssdesc;

void gdt_set_entry(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity);
void gdt_init();
extern void s_setgdt(uint32_t limit, uint64_t base);
extern void s_flushgdt(void);
extern void s_settss(void);