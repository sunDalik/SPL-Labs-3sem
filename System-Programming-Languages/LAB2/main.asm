%include "colon.inc"
%include "words.inc"
extern find_word
extern read_word
extern print_string
extern string_length
extern print_newline
global _start

section .rodata
error_string: db "No such word found!", 0
error_string_length: db 20

section .text

print_error:
	mov rsi, error_string
    mov rdx, [error_string_length]        ;число байтов для печати 
    mov rax, 1          ;номер системного вызова write
    mov rdi, 2          ;2 - stderr
    syscall
    ret

_start:
	sub rsp, 256
	mov rdi, rsp
	mov rsi, 255
	call read_word
	test rax,rax
	je exit_with_error
	mov rdi, rax
	mov rsi,last_word
	call find_word
	cmp rax, 0
	je exit_with_error
	lea rdi, [rax+8]
	call string_length
	lea rdi, [rdi+rax+1]
	call print_string
	call print_newline
	mov rax, 60
	mov rdi, 0
	syscall
	exit_with_error:
	call print_error
	call print_newline
	mov rax, 60
	mov rdi, 1
	syscall