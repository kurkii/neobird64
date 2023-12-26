#include "../../include/limine.h"
#include "acpi.h"
#include "apic.h"
#include <log.h>
#include <stdio.h>
#include <string.h>

rsdt_t *rsdt;
xsdt_t *xsdt;
fadt_t *fadt;

#define PMT_TIMER_RATE 3579545 // 3.57 MHz

extern uint64_t hhdmoffset;

static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};


////////////////////////////// Parsing functions //////////////////////////////

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

fadt_t *fetch_fadt(){
    return find_acpi_table("FACP", rsdt, xsdt);
}


void *find_acpi_table(char signature[4], rsdt_t *rsdt, xsdt_t *xsdt){
    int usexsdt;
    int entries;
    printf("acpi: looking for table with signature '{cccc}'{n}", signature[0], signature[1], signature[2], signature[3]);
    if(!xsdt){                                                      // checks if the XSDT is available
        usexsdt = 0;
        entries = (rsdt->header.length - sizeof(sdt_t))/ 4;
    }else{
        usexsdt = 1;
        entries = (xsdt->header.length - sizeof(sdt_t))/ 8;
    }

    for(int i = 0; i < entries; i++){
        sdt_t *header;
        if(usexsdt == 0){
            header = (sdt_t*)rsdt->tableptrs[i];
        }else{
            header = (sdt_t*)xsdt->tableptrs[i];
        }
        
        if(!memcmp(header->signature, signature, 4)){
            printf("acpi: Found table with signature '{cccc}'{n}", signature[0], signature[1], signature[2], signature[3]);
            return (void*)header;
        }
    }
            
    printf("acpi: Table '{s}' not found.{n}", signature);
    return NULL;  
}

////////////////////////////// Initialization functions //////////////////////////////

void init_acpi(void){
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
    
    init_apic(madt, hhdmoffset);

}

////////////////////////////// ACPI Timer Delay //////////////////////////////

void pmt_delay(size_t us){
    if(fadt->PMTimerLength != 4){
        log_panic("ACPI Timer unavailable"); // panic for now
    }
    uint64_t* addr = (uint64_t*)fadt->X_PMTimerBlock.Address;
    size_t count = inl(fadt->PMTimerBlock);
    size_t target = (us*PMT_TIMER_RATE)/1000000;
    size_t current = 0;

    while (current < target) {
        current = ((inl(fadt->PMTimerBlock) - count) & 0xffffff);
    }

}

