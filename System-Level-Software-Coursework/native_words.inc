;---convenient macros---
%define _lw 0
%macro native 3
    section .data
    wh_ %+ %2 : dq _lw
    db 0
    db %1, 0
    db %3
    
    %define _lw wh_%+ %2 
    xt_ %+ %2 :  dq i_ %+ %2 
    section .text
    i_ %+ %2:
%endmacro

%macro native 2
native %1, %2, 0
%endmacro


;---stack combinators?---

;last letter [b in (a b)] in stack diagrams	marks top of the stack 
; (a b -- a) 
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