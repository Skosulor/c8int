do:
	gcc -g -c emu.c -o emu.o
	gcc -g -c chip8.c -o chip8.o
	gcc emu.o chip8.o -o output




