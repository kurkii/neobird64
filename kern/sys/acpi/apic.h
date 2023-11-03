#pragma once

#include "acpi.h"

void init_apic(madt_t *madt, uint64_t hhdmoffset);
void apic_eoi(void);
int apic_read(void *ioapicaddr, int reg);
void apic_write(void *ioapicaddr, int reg, int value);