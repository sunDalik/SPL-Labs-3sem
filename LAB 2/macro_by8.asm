section .data
from1: db "12345678901234567890", 0
to1: resb 21

section .text
extern print_string
extern print_newline


;%1 - откуда  
;%2 - куда 
;%3 - n
%macro macro_copy_big_data 3
  %assign counter8 0
  %rep (%3 / 8)
    mov r8, [%1+8*counter8]
    mov [%2+8*counter8], r8
    %assign counter8 (counter8)+1
  %endrep
  %assign counter1 0
  %rep (%3 % 8)
    mov r8b, [%1+8*counter8+counter1]
    mov [%2+8*counter8+counter1], r8b
    %assign counter1 (counter1)+1
  %endrep
%endmacro


%include "bench.inc"

global _start
_start:
  call test

  mov rax, 60
  mov rdi, 0
  syscall