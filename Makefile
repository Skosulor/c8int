do:
	gcc -c emu.c -o emu.o
	gcc -c chip8.c -o chip8.o
	gcc emu.o chip8.o -o output




