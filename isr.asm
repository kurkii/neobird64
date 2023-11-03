
global s_isr0
global s_isr1
global s_isr2
global s_isr3
global s_isr4
global s_isr5
global s_isr6
global s_isr7
global s_isr8
global s_isr9
global s_isr10
global s_isr11
global s_isr12
global s_isr13
global s_isr14
global s_isr15
global s_isr16
global s_isr17
global s_isr18
global s_isr19
global s_isr20
global s_isr21
global s_isr22
global s_isr23
global s_isr24
global s_isr25
global s_isr26
global s_isr27
global s_isr28
global s_isr29
global s_isr30
global s_isr31

s_isr0:
    ;cli
    push byte 0     ; dummy
    push byte 0     ; isr num
    jmp isr_common_stub

s_isr1:
    cli
    push byte 0     ; dummy
    push byte 1     ; isr num
    jmp isr_common_stub

s_isr2:
    cli
    push byte 0     ; dummy
    push byte 2     ; isr num
    jmp isr_common_stub

s_isr3:
    cli
    push byte 0     ; dummy
    push byte 3     ; isr num
    jmp isr_common_stub

s_isr4:
    cli
    push byte 0     ; dummy
    push byte 4     ; isr num
    jmp isr_common_stub

s_isr5:
    cli
    push byte 0     ; dummy
    push byte 5     ; isr num
    jmp isr_common_stub

s_isr6:
    cli
    push byte 0     ; dummy
    push byte 6     ; isr num
    jmp isr_common_stub

s_isr7:
    cli
    push byte 0     ; dummy
    push byte 7     ; isr num
    jmp isr_common_stub

s_isr8:
    cli
                    ; dont push dummy as it already pushes one
    push byte 8     ; isr num
    jmp isr_common_stub

s_isr9:
    cli
    push byte 0     ; dummy
    push byte 9     ; isr num
    jmp isr_common_stub

s_isr10:
    cli
                    ; dont push dummy as it already pushes one
    push byte 10     ; isr num
    jmp isr_common_stub

s_isr11:
    cli
                    ; dont push dummy as it already pushes one
    push byte 11     ; isr num
    jmp isr_common_stub

s_isr12:
    cli
                    ; dont push dummy as it already pushes one
    push byte 12     ; isr num
    jmp isr_common_stub

s_isr13:
    cli
                    ; dont push dummy as it already pushes one
    push byte 13     ; isr num
    jmp isr_common_stub

s_isr14:
    cli
                    ; dont push dummy as it already pushes one
    push byte 14     ; isr num
    jmp isr_common_stub

s_isr15:
    cli
    push byte 0      ; dummy
    push byte 15     ; isr num
    jmp isr_common_stub

s_isr16:
    cli
    push byte 0      ; dummy
    push byte 16     ; isr num
    jmp isr_common_stub

s_isr17:
    cli
    push byte 0      ; dummy
    push byte 17     ; isr num
    jmp isr_common_stub

s_isr18:
    cli
    push byte 0      ; dummy
    push byte 18     ; isr num
    jmp isr_common_stub

; 19: Reserved
s_isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
s_isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
s_isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
s_isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
s_isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
s_isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
s_isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
s_isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
s_isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
s_isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
s_isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
s_isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
s_isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.

extern exception_handler
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, exception_handler
    call eax       ; A special call, preserves the 'eip' register

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!




