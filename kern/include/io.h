#include <stdint.h>

void wrmsr(uint64_t msr, uint64_t value);
uint64_t rdmsr(uint64_t msr_id);
void write_reg(uintptr_t addr, uint32_t val );