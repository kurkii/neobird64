default rel

global s_setgdt
global s_flushgdt
global s_settss

extern gdtr
 
s_setgdt:
   mov   [gdtr], DI
   mov   [gdtr+2], RSI
   lgdt  [gdtr]
   mov rax, 0x10       ; kernel data offset
   mov ds, rax
   mov es ,rax
   mov gs, rax
   mov fs, rax
   mov ss, rax
   pop rax
   push 0x08            ; kernel code offset
   push rax
   retfq

s_settss:
   mov ax, 0x28   ; tss offset in our gdt
   ltr ax
   ret