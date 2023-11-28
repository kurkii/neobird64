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

typedef struct{

  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;

}__attribute((packed))gas_t;

typedef struct{
    sdt_t header;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;
 
    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;
 
    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;
 
    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;
 
    uint8_t  Reserved2;
    uint32_t Flags;
 
    // 12 byte structure; see below for details
    gas_t ResetReg;
 
    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
 
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;
 
    gas_t X_PM1aEventBlock;
    gas_t X_PM1bEventBlock;
    gas_t X_PM1aControlBlock;
    gas_t X_PM1bControlBlock;
    gas_t X_PM2ControlBlock;
    gas_t X_PMTimerBlock;
    gas_t X_GPE0Block;
    gas_t X_GPE1Block;
}__attribute((packed))fadt_t;

typedef struct {
    uint32_t type;
    madt_record_t header;
} __attribute((packed)) ics_list_t;

int acpi_table_checksum(xsdt_t *t);
void *find_acpi_table(char signature[4], rsdt_t *rsdt, xsdt_t *xsdt);
void init_acpi(void);
void pmt_delay(int us);
