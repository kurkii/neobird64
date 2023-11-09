#pragma once

#include <stdint.h>
#include "../../include/limine.h"

typedef struct {
    uint8_t signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t OEMtableid[8];
    uint32_t oemrevision;
    uint32_t creatorid;
    uint32_t creatorrevision;
} __attribute((packed)) sdt_t;          // standard ACPI header structure


typedef struct {
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t rsdtaddress;
    uint32_t length;
    uint64_t xsdtaddress;
    uint8_t extendedchecksum;
    uint8_t reserved[3];
} __attribute((packed)) rsdp_t;         

typedef struct {
    sdt_t header;
    uint64_t tableptrs[];

} __attribute((packed)) xsdt_t;

typedef struct {
    sdt_t header;
    uint32_t tableptrs[];

} __attribute((packed)) rsdt_t;


typedef struct {
    uint8_t entry_type;
    uint8_t record_length;
} __attribute((packed)) madt_record_t;

typedef struct {                        // Processor LAPIC struct 
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t processorUID;
    uint8_t apicID;
    uint32_t flags;
} __attribute((packed)) madt_plapic_t;

typedef struct {                        // IOAPIC struct
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t ioapicID;
    uint8_t reserved;
    uint32_t ioapicaddr;
    uint32_t gsi;
} __attribute((packed)) madt_ioapic_t;

typedef struct {                        // Interrupt Source Override
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t bus;
    uint8_t source;
    uint32_t gsi;
    uint16_t flags;
} __attribute((packed)) madt_iso_t;


typedef struct {                        // Non-Maskable Interrupt struct 
    uint8_t entry_type;
    uint8_t entry_length;
    uint16_t flags;
    uint32_t gsi;
} __attribute((packed)) madt_nmi_t;

typedef struct {                        // LAPIC NMI struct
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t processorUID;
    uint16_t flags;
    uint8_t lint_num;
} __attribute((packed)) madt_lapicnmi_t;

typedef struct {
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t ioapicID;
    uint8_t reserved;
    uint32_t gsi;
    uint64_t iosapicaddr;
} __attribute((packed)) madt_iosapic_t;

typedef struct {
    uint8_t entry_type;
    uint8_t entry_length;
    uint8_t processorUID;
    uint8_t lsapicID;
    uint8_t lsapicEID;
    uint8_t reserved[3];
    uint32_t flags;
    uint32_t processorUIDval;
    uint8_t processorUIDstr;
} __attribute((packed)) madt_pslapic_t;

typedef struct {
    sdt_t header;

    uint32_t lapicaddr;
    uint32_t flags;

    madt_record_t first_ics;
} __attribute((packed)) madt_t;

typedef struct {
    uint32_t type;
    madt_record_t header;
} __attribute((packed)) ics_list_t;

int acpi_table_checksum(xsdt_t *t);
void *find_acpi_table(char signature[4], rsdt_t *rsdt, xsdt_t *xsdt);
void init_acpi(void);
