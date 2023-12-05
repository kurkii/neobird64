default rel

extern exception_handler

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
global s_isr172

s_isr0:
    ;
    push qword 0     ; dummy
    push qword 0     ; isr num
    jmp isr_common_stub

s_isr1:
    
    push qword 0     ; dummy
    push qword 1     ; isr num
    jmp isr_common_stub

s_isr2:
    
    push qword 0     ; dummy
    push qword 2     ; isr num
    jmp isr_common_stub

s_isr3:
    
    push qword 0     ; dummy
    push qword 3     ; isr num
    jmp isr_common_stub

s_isr4:
    
    push qword 0     ; dummy
    push qword 4     ; isr num
    jmp isr_common_stub

s_isr5:
    
    push qword 0     ; dummy
    push qword 5     ; isr num
    jmp isr_common_stub

s_isr6:
    
    push qword 0     ; dummy
    push qword 6     ; isr num
    jmp isr_common_stub

s_isr7:
    
    push qword 0     ; dummy
    push qword 7     ; isr num
    jmp isr_common_stub

s_isr8:
    
                    ; dont push dummy as it already pushes one
    push qword 8     ; isr num
    jmp isr_common_stub

s_isr9:
    
    push qword 0     ; dummy
    push qword 9     ; isr num
    jmp isr_common_stub

s_isr10:
    
                    ; dont push dummy as it already pushes one
    push qword 10     ; isr num
    jmp isr_common_stub

s_isr11:
    
                    ; dont push dummy as it already pushes one
    push qword 11     ; isr num
    jmp isr_common_stub

s_isr12:
    
                    ; dont push dummy as it already pushes one
    push qword 12     ; isr num
    jmp isr_common_stub

s_isr13:
    
                    ; dont push dummy as it already pushes one
    push qword 13     ; isr num
    jmp isr_common_stub

s_isr14:
    
                    ; dont push dummy as it already pushes one
    push qword 14     ; isr num
    jmp isr_common_stub

s_isr15:
    
    push qword 0      ; dummy
    push qword 15     ; isr num
    jmp isr_common_stub

s_isr16:
    
    push qword 0      ; dummy
    push qword 16     ; isr num
    jmp isr_common_stub

s_isr17:
    
    push qword 0      ; dummy
    push qword 17     ; isr num
    jmp isr_common_stub

s_isr18:
    
    push qword 0      ; dummy
    push qword 18     ; isr num
    jmp isr_common_stub

; 19: Reserved
s_isr19:
    
    push qword 0
    push qword 19
    jmp isr_common_stub

; 20: Reserved
s_isr20:
    
    push qword 0
    push qword 20
    jmp isr_common_stub

; 21: Reserved
s_isr21:
    
    push qword 0
    push qword 21
    jmp isr_common_stub

; 22: Reserved
s_isr22:
    
    push qword 0
    push qword 22
    jmp isr_common_stub

; 23: Reserved
s_isr23:
    
    push qword 0
    push qword 23
    jmp isr_common_stub

; 24: Reserved
s_isr24:
    
    push qword 0
    push qword 24
    jmp isr_common_stub

; 25: Reserved
s_isr25:
    
    push qword 0
    push qword 25
    jmp isr_common_stub

; 26: Reserved
s_isr26:
    
    push qword 0
    push qword 26
    jmp isr_common_stub

; 27: Reserved
s_isr27:
    
    push qword 0
    push qword 27
    jmp isr_common_stub

; 28: Reserved
s_isr28:
    
    push qword 0
    push qword 28
    jmp isr_common_stub

; 29: Reserved
s_isr29:
    
    push qword 0
    push qword 29
    jmp isr_common_stub

; 30: Reserved
s_isr30:
    
    push qword 0
    push qword 30
    jmp isr_common_stub

; 31: Reserved
s_isr31:
    
    push qword 0
    push qword 31
    jmp isr_common_stub

s_isr172:
    push qword 0
    push qword 172
    jmp isr_common_stub

%macro pushaq 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

%endmacro

%macro popaq 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro


isr_common_stub:
    pushaq
    mov rdi, rsp
    call exception_handler
    popaq
    add rsp, 16
    iretq