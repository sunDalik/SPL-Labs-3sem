global _start
%include "lib.inc"

%define pc r15
%define w r14
%define rstack r13

section .text
%include "native_words.inc"
%include "colon.inc"

section .bss
resq 1023
rstack_start: resq 1
user_dict: resq 65536
user_mem: resq 65536

section .data
last_word: dq _lw
here: dq user_dict
dp: dq user_mem
stack_start:  dq 0
 
section .text
_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp
    mov pc, forth_init

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]

