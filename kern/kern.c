#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "limine.h"
#include "video/video.h"
#include "video/font.c"
#include "flanterm/flanterm.h"
#include "flanterm/backends/fb.h"
#include "video/printf.h"

 
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
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

struct flanterm_context *ft_ctx;


void _start(void) {
    
    struct flanterm_context *ft_ctx = fb_init();
    const char msg[] = "Hello world\n";

    printf("hi");

    hcf();
}
