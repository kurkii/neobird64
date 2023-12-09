#include <stdint.h>

void wrmsr(uint64_t msr, uint64_t value);
uint64_t rdmsr(uint64_t msr_id);
void write_reg(uintptr_t addr, uint32_t val );
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
uint32_t inl(uint16_t port);
void sleep(int ms);