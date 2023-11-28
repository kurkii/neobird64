#include "../../include/limine.h"
#include "acpi.h"
#include "apic.h"
#include <log.h>
#include <stdio.h>
#include <string.h>

#define PMT_TIMER_RATE 3579545 // 3.57 MHz

rsdt_t *rsdt;
xsdt_t *xsdt;
fadt_t *fadt;
uint64_t hhdmoffset;
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};



xsdt_t *parse_xsdt(uint64_t hhdmoffset, rsdp_t *rsdp){
    if(!rsdp->xsdtaddress){
        return NULL;
    }
    return (xsdt_t*)(rsdp->xsdtaddress + hhdmoffset);
}

rsdt_t *parse_rsdt(uint64_t hhdmoffset, rsdp_t *rsdp){
    if(!rsdp->rsdtaddress){
        return NULL;
    }
    return (rsdt_t*)(rsdp->rsdtaddress + hhdmoffset);
}

void pmt_delay(size_t us){
    if(fadt->PMTimerLength != 4){
        log_panic("ACPI Timer unavailable"); // panic for now
    }
    printf("bruj");
    uint64_t* addr = (uint64_t*)fadt->X_PMTimerBlock.Address;
    printf("bru1j");
    size_t count = inl(fadt->PMTimerBlock);\
    printf("bru2j");
    size_t target = (us*PMT_TIMER_RATE)/1000000;
    printf("bru3j");
    size_t current = 0;

    while (current < target) {
        current = ((inl(fadt->PMTimerBlock) - count) & 0xffffff);
    }
    

}

fadt_t *fetch_fadt(uint64_t hhdmoffset){
    return find_acpi_table("FACP", rsdt, xsdt);
}

void init_acpi(void){
    hhdmoffset = hhdm_request.response->offset;
    rsdp_t *rsdp = (rsdp_t*)rsdp_request.response->address;

    rsdt = parse_rsdt(hhdmoffset, rsdp);
    xsdt = parse_xsdt(hhdmoffset, rsdp);

    madt_t *madt = find_acpi_table("APIC", rsdt, xsdt);       // APIC - signature of MADT table

    if(madt == NULL){
        log_panic("MADT table not found, hanging");
    }

    fadt = find_acpi_table("FACP", rsdt, xsdt);

    if(fadt == NULL){
       log_panic("FADT table not found, hanging");
    }

    printf("{x}", hhdmoffset);
    init_apic(madt, hhdmoffset);


    
}

void *find_acpi_table(char signature[4], rsdt_t *rsdt, xsdt_t *xsdt){
    int usexsdt;
    int entries;

    if(!xsdt){                                                      // checks if the XSDT is available
        usexsdt = 0;
        entries = (rsdt->header.length - sizeof(sdt_t))/ 4;
        printf("acpi: using the RSDT{n}");
        printf("acpi: revision: {dn}", rsdt->header.revision);
        printf("acpi: OEMID: {sn}", rsdt->header.OEMID);
    }else{
        usexsdt = 1;
        entries = (xsdt->header.length - sizeof(sdt_t))/ 8;
        printf("acpi: using the XSDT{n}");
        printf("acpi: revision: {dn}", xsdt->header.revision);
        printf("acpi: OEMID: {ccccccn}", xsdt->header.OEMID[0], xsdt->header.OEMID[1], xsdt->header.OEMID[2], xsdt->header.OEMID[3], xsdt->header.OEMID[4], xsdt->header.OEMID[5]);
    }

    printf("acpi: ACPI entries: {dn}", entries);
    for(int i = 0; i < entries; i++){
        sdt_t *header;
        if(usexsdt == 0){
            header = (sdt_t*)rsdt->tableptrs[i];
        }else{
            header = (sdt_t*)xsdt->tableptrs[i];
        }
        
        printf("debug: table[{i}]: '{cccc}'{n}", i, header->signature[0], header->signature[1], header->signature[2], header->signature[3]);
        if(!memcmp(header->signature, signature, 4)){
            printf("acpi: Found table with signature '{cccc}'{n}", signature[0], signature[1], signature[2], signature[3]);
            return (void*)header;
        }
    }
            
    printf("acpi: Table '{s}' not found.{n}", signature);
    return NULL;  
}

