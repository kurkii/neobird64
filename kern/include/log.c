#include "log.h"
#include "video.h"
#include <stdio.h>
void log_panic(char *msg){
    printf("{k}kern_panic: {ksn}", ANSI_COLOR_RED, ANSI_COLOR_RESET, msg);
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void log_err(char *msg){
    printf("{k}kern_err: {ksn}", ANSI_COLOR_RED, ANSI_COLOR_RESET, msg);

}

void log_warn(char *msg){
    printf("{k}kern_warn: {ksn}", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, msg);
}

void log_info(char *msg){
    printf("{k}kern_info: {ksn}", ANSI_COLOR_CYAN, ANSI_COLOR_RESET, msg);
}

void log_success(char *msg){
    printf("{k}kern_success: {ksn}", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, msg);
}