all:
	nasm -f elf64 -g main.asm
	ld -o forth *.o
	
clean: 
	rm *.o
	rm forth