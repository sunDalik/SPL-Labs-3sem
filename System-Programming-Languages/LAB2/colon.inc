%define last_word 0

%macro colon 2	;аргумент один - ключ в виде строки, второй аргумент ключ как метка
%%last_word: dq last_word
db %1, 0
%2:
%define last_word %%last_word
%endmacro