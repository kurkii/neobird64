#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <video.h>
#include <log.h>
#include <io.h>
#include <neobird64.h>
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"
#include "limine.h"
#include "sys/gdt.h"
#include "sys/idt.h"
#include "sys/acpi/acpi.h"
#include "sys/ps2/ps2.h"
#include "mm/pmm.h"
#include "mm/vmm.h"
#include "sys/smp/smp.h" 
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};



struct flanterm_context *fb_init(){
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
    || framebuffer_request.response->framebuffer_count < 1) {
        
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    struct flanterm_context *ft_ctx = flanterm_fb_simple_init(
        framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch
    );
    return ft_ctx;
}


static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

struct flanterm_context *ft_ctx;


void _start(void) {
    ft_ctx = fb_init();
    
    printf("{k}Welcome to Neobird64 v{skn}", ANSI_COLOR_CYAN, NEOBIRD64_VERSION_MAJOR, ANSI_COLOR_RESET);
    log_info("Loading GDT...");
    gdt_init();
    log_success("GDT loaded!");
    log_info("Loading IDT...");
    idt_init();
    log_success("IDT loaded!");
    log_info("Loading PMM...");
    pmm_init();
    log_success("PMM loaded!");
    log_info("Initalizing ACPI and APIC..."); 
    init_acpi();
    log_info("Loading VMM...");
    vmm_init();
    log_success("VMM loaded!");
    log_info("Starting CPUs...");
    smp_init();
    printf("Done!{n}");
    printf("Hi!{n}");
    // We're done, just hang...
    hcf(); 

}