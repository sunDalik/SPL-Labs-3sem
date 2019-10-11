section .text

global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy

string_length:
    xor rax, rax
    string_length_loop:
        mov r8b, [rdi+rax] ;передаем следующий символ
        cmp r8b, 0x0       ;если конец ввода то выходим
        je string_length_exit
        inc rax            ;инкрементируем счетчик длины строки
        jmp string_length_loop
    string_length_exit:
        ret


print_string:
    xor rax, rax
    mov rsi, rdi        ;начало строки
    call string_length  
    mov rdx, rax        ;число байтов для печати определяется функцией подсчета длины
    mov rax, 1          ;номер системного вызова write
    mov rdi, 1          ;1 - stdout
    syscall
    ret


print_char:
    push rdi            ;символ надо сначала загрузить в память
    mov rax, 1          ;номер системного вызова write
    mov rsi, rsp        ;начало строки на вершине стека
    mov rdx, 1          ;печатаем 1 байт   
    mov rdi, 1          ;1 - stdout
    syscall
    pop rdi             ;возвращаем код возврата на вершину стека
    ret


print_newline:
    xor rax, rax
    mov rdi, 0xA        ;символ новой строки
    call print_char
    ret


print_uint:
    mov rax, rdi
    mov r9, rsp     ;сохраняем стек
    dec rsp 
    mov byte [rsp], 0x0  ;кладем на вершину стека символ конца ввода
    mov r8, 10          ;делитель
    print_uint_loop:
        mov rdx, 0x0    
        div r8          ;делим на 10 число
        dec rsp         
        add rdx, 0x30   ;переводим цифру в символ
        mov [rsp], dl   ;кладем остаток, который хранится в rdx на вершину стека
        cmp rax, 0x0
        je print_uint_exit  ;если конец ввода то выходим
        jmp print_uint_loop
    print_uint_exit:
        mov rdi, rsp 
        call print_string   ;выводим строку, начало которой находится в вершине стека
        mov rsp, r9     ;возвращаем стек
        ret


print_int:
    xor rax, rax
    test rdi, rdi 
    jns print_int_if_positive ;если число положительное, то просто вызываем print_uint
    push rdi
    mov rdi, '-'            
    call print_char           ;если число отрицательное, то сначала печатаем минус
    pop rdi
    neg rdi                   ;переводим отрицательное число в положительное
    print_int_if_positive:
        call print_uint
        ret


string_equals:
    xor rax, rax
    xor rcx, rcx
string_equals_loop:
    mov r8b, byte [rdi+rcx]  ;r8 - символ первой строки
    mov r9b, byte [rsi+rcx]  ;r9 - символ второй строки
    cmp r8b, r9b
    jne string_equals_false  ;если они не равны то возвращаем 0
    inc rcx                  ;инкрементируем счетчик
    cmp r8b, 0x0
    je string_equals_true   ;строка кончилась, возвращаем 1
    jmp string_equals_loop
string_equals_true:
    mov rax, 1      
    ret
string_equals_false:
    mov rax, 0     
    ret


read_char:
    mov rax, 0        ;код системного вызова read
    mov rdi, 0        ;0-stdin
    mov rdx, 1        ;число байтов для считывания
    dec rsp
    mov rsi, rsp      ;куда считывать
    syscall
    test rax, rax     ;системный вызов записываем количество считанных символов в rax
    je read_char_exit ;если оно равно 0, то конец строки
    mov al, [rsp]
    read_char_exit:   
        inc rsp       ;возвращаем на вершину стека адрес возврата  
        ret 


read_word:
        xor r8, r8      ;счетчик символа
        mov r9, rdi     ;начало буфера
        mov r10, rsi    ;размер буфера
    read_word_first_loop:
        call read_char
        cmp al, 0x20
        je read_word_first_loop
        cmp al, 0x9
        je read_word_first_loop   ;пробелы и табы перед словом не считываем
        cmp al, 0xA				  
        je read_word_first_loop	  
    write_char:
        cmp al, 0x9     ;Таб
        je read_word_exit
        cmp al, 0x0     ;Конец ввода
        je read_word_exit
        cmp al, 0x20    ;Пробел
        je read_word_exit
        cmp al, 0xA				  
        je read_word_exit	  
        mov byte [r9+r8], al    ;записываем новый символ в буфер
        inc r8
        cmp r8, r10     
        je read_word_of     ;переполнение строки
        call read_char      ;считываем новый символ
        jmp write_char
    read_word_exit:
        mov rax, r9     ;в выводе начало буфера
        mov byte [r9+r8], 0     ;добавляем NULL
        mov rdx, r8     ;длина строки
        ret
    read_word_of:
        xor rax, rax    ;0 обозначает ошибку
        xor rdx, rdx    ;0 обозначает ошибку
    ret


; rdi-начало строки
; возвращает rax:число, rdx:длина
parse_uint:
        xor rax, rax    
        xor rsi, rsi    ;цифра
        mov r8, 10      ;множитель
        xor rcx, rcx    ;счетчик символа
        xor rdx, rdx
    parse_uint_loop:
        mov sil, [rdi+rcx]  ;считываем новую цифру
        cmp sil, 0x30
        jl parse_uint_exit
        cmp sil, 0x39
        jg parse_uint_exit  ;проверяем, что символ в пределах 0-9, а иначе выходим
        inc rcx
        sub sil, 0x30       ;переводим символ в цифру
        mul r8              ;умножаем на 10, чтобы в младший ноль записать новую цифру
        add rax, rsi    
        jmp parse_uint_loop
    parse_uint_exit:
        mov rdx, rcx
    ret


; rdi-начало строки
; возвращает rax:число, rdx:длина
parse_int:
    xor rax, rax
    cmp byte [rdi], 0x2d 
    je parse_int_negative ;если первый символ "-", то переходим к обработке отрицательного числа
    call parse_uint
    ret
parse_int_negative:
    inc rdi         ;пропускаем минус
    call parse_uint
    cmp rdx, 0      ;если не считали цифр, то не учитываем "-" в длине
    je parse_int_exit
    neg rax         ;переводим в отрицательное число
    inc rdx         ;+1 к длине из-за минуса
parse_int_exit:
    ret 


;rdx - размер буфера
;rdi - откуда копировать
;rsi - куда копировать
;возвращает rax:начало скопированной строки
string_copy:
    xor rcx, rcx
    string_copy_loop:
        cmp rcx, rdx
        je string_copy_error_exit ;переполнение буфера
        mov r8b, [rdi+rcx]        ;копируем
        mov [rsi+rcx], r8b        ;вставляем
        inc rcx                   ;счетчик символа
        cmp r8b, 0x0
        je string_copy_exit       ;выходим, если конец строки
    jmp string_copy_loop
    string_copy_exit:
        mov rax, rsi
        ret
    string_copy_error_exit:
        xor rax, rax  ;0 обозначает ошибку
        ret