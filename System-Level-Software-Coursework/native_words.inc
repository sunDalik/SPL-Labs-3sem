;---convenient macros---
%define _lw 0
%macro native 3 ;%1 - word name, %2 - word label, %3 - immediate flag
    section .data
    wh_ %+ %2 : dq _lw
    db 0, %1, 0, %3
    %define _lw wh_%+ %2 
    xt_ %+ %2 :  dq i_ %+ %2 
    section .text
    i_ %+ %2:
%endmacro

%macro native 2 ;immediate flag is 0 by default
native %1, %2, 0
%endmacro

;---return stack macros---

%macro rspush 1
    sub rstack, 8
    mov qword [rstack], %1
%endmacro

%macro rspop 1
    mov %1, qword [rstack]
    add rstack, 8
%endmacro


;---stack combinators?---

;last letter [c in (a b c)] in stack diagrams marks top of the stack 
;(a b -- a) 
native "drop", drop
    add rsp, 8
    jmp next

;(a b -- b a)
native "swap", swap
    pop rax
    pop rdx
    push rax
    push rdx
    jmp next

;(a -- a a)
native "dup", dup
    push qword [rsp]
    jmp next

native ".", pop_top
    pop rdi
    call print_int
    call print_newline
    jmp next

native ".S", show_stack
    mov rcx, rsp
    .show_stack_loop:
        cmp rcx, [stack_start] 
        jae next
        mov rdi, [rcx]
        push rcx
        call print_int
        call print_newline
        pop rcx
        add rcx, 8
        jmp .show_stack_loop
    
    
;---arithmetic operators---

native "+", plus
    pop rax
    add [rsp], rax
    jmp next

native "*", mul
    pop rax
    pop rdx
    imul rdx
    push rax
    jmp next

;(a b -- a/b) integer division
;notice that stack top is the second operand in arithmetic operations
native "/", div
    pop rcx
    pop rax
    xor rdx, rdx
    idiv rcx
    push rax
    jmp next

;(a b -- a%b) remainder of division
native "%", mod
    pop rcx
    pop rax
    xor rdx, rdx
    idiv rcx
    push rdx
    jmp next

;(a b -- a-b) 
native "-", minus
    pop rax
    sub [rsp], rax
    jmp next

native "=", equals
    pop rax
    pop rdx
    cmp rax, rdx
    je .equals_true
    push 0
    jmp next
    .equals_true:
        push 1
        jmp next

;(a b -- a<b)
native "<", less
    pop rdx
    pop rax
    cmp rax, rdx
    jl .less_true
    push 0
    jmp next
    .less_true:
        push 1
        jmp next
    

;---logical operators---
    
native "not", not
    pop rax
    test rax, rax
    je .not_zero
    xor rax, rax
    push rax
    jmp next
    .not_zero:
        mov rax, 1
        push rax
        jmp next

;bitwise and
native "and", and
    pop rax
    and [rsp], rax
    jmp next

;bitwise or
native "or", or
    pop rax
    or [rsp], rax
    jmp next
    
;logical and, expects both arguments to be 1 or 0
native "land", land
    pop rax
    pop rdx
    test rax, rax
    je .land_false
    push rdx
    jmp next
.land_false:
    push rax
    jmp next

;logical or, expects both arguments to be 1 or 0
native "lor", lor
    pop rax
    pop rdx
    test rax, rax
    jne .lor_true
    push rdx
    jmp next
.lor_true:
    push rax
    jmp next


;---return stack operators---

;place value from data stack into return stack
native ">r", to_rs
    pop rax
    rspush rax
    jmp next

;place value from return stack to data stack
native "r>", from_rs
    rspop rax
    push rax
    jmp next

;copy value from return stack to data stack
native "r@", from_rs_copy
    push qword [rstack]
    jmp next
    

;---memory management---

;(addr -- val) fetch value by address
native "@", fetch_mem
    pop rax
    push qword[rax]
    jmp next

;(val addr -- ) store value by address 
native "!", write_mem
    pop rax
    pop rdx
    mov [rax], rdx
    jmp next

;(char addr -- ) store one byte by address
native "c!", write_char_mem
    pop rax
    pop rdx
    mov [rax], dl
    jmp next

;(addr -- char) fetch one byte by address
native "c@", fetch_char_mem
    pop rax
    movzx rax, byte [rax]
    push rax
    jmp next

;(xt -- ) execute word by execution token
native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]
    

;---execution management---

native "docol", docol
    rspush pc
    add w, 8
    mov pc, w
    jmp next

native "branch", branch
    mov pc, [pc]
    jmp next

native "0branch", branch0
    pop rax
    test rax, rax
    je .branch0_true
    add pc, 8
    jmp next
    .branch0_true:
        mov pc, [pc]
        jmp next
    
native "exit", exit
    rspop pc
    jmp next


;---service---

native "lit", lit
    push qword [pc]
    add pc, 8
    jmp next
    
;(syscall_number arg1 arg2 arg3 arg4 arg5 arg6 -- new_rax new_rdx )
native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    push rdx
    jmp next
    
native "sp", sp
    push rsp
    jmp next

native "here", here
    push qword here
    jmp next
	
native "here_ind", here_ind
    push qword [here]
    jmp next

native "IMMEDIATE", immediate
    mov rax, [last_word]
    mov dl, byte [rax + 8]
    or dl, 1
    mov byte [rax + 8], dl
    jmp next