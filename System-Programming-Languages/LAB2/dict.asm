extern string_equals
global find_word

find_word:		;rdi  ключ   rsi последнее слово
	push r12
	mov r12, rsi
	find_word_loop:
		cmp r12, 0		;если указателя не было то это было последнее слово и мы ниче не нашли
		je find_word_false
		lea rsi, [r12+8]	;прибавляем 8 чтобы перейти к ключу
		call string_equals
		cmp rax, 1		
		je find_word_true
		mov r12, [r12]	;указывваем на следующее слово
		jmp find_word_loop
	find_word_true:
		mov rax, r12
		pop r12
		ret
	find_word_false:
		mov rax, 0
		pop r12
		ret