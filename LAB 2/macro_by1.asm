section .data
from1: db "12345678901234567890", 0
to1: resb 21

section .text
extern print_string
extern print_newline


;%1 - откуда  
;%2 - куда 
;%3 - n
%macro macro_copy_data 3
	%assign counter 0
	%rep %3
		mov r8b, [%1+counter]
		mov [%2+counter], r8b
		%assign counter (counter)+1
	%endrep
%endmacro


%include "bench0.inc"

global _start
_start:
	call test

	mov rax, 60
	mov rdi, 0
	syscall