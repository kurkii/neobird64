#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <video.h>
#include <log.h>
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"
#include "limine.h"
#include "sys/gdt/gdt.h"
#include "sys/idt/idt.h"
#include "sys/acpi/acpi.h"
#include "sys/acpi/apic.h"
#include "sys/keyboard/ps2.h"
 
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
    const char msg[] = "Hello world\n";
    printf("{k}Welcome to Neobird64{kn}", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    log_info("Loading GDT...");
    gdt_init();
    log_success("GDT loaded!");
    log_info("Loading IDT...");
    idt_init();
    log_success("IDT loaded!");
    printf("Hi!{n}");
    init_acpi();
    // We're done, just hang...
    hcf();

}