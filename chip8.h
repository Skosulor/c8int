#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE  1

#define RAM_SIZE      0xFFF
#define PROGRAM_START 0x200

/* Instruction masking
   0XYN
   0XNN
   0NNN */
#define GET_X(X)   ((X & 0x0F00) >> 8)
#define GET_Y(X)   ((X & 0x0F00) >> 4)
#define GET_NNN(X) ((X & 0x0FFF))
#define GET_NN(X)  ((X & 0x00FF))
#define GET_N(X)   ((X & 0x000F))

typedef uint8_t bool;

/* emulated ram memory */
uint8_t memory[RAM_SIZE];
/* 2048 pixels in total
bool pixel[64][32];
/* General purpose registers
   Named Vx (V0-VF)
   VF reserved */
uint8_t  V[16];
uint16_t stack[16];
/* stack pointer */
uint8_t  sp;
/* Memory size is 4KB and needs a 16-bit pc */
uint16_t pc;
/* Index register */
uint16_t i_reg;
/* operation */
uint16_t op_code;


/* Updates at 60 Hz */
uint16_t delay_timer;
uint16_t sound_timer;

bool keypad[16];

void init();
void load_game();
void print_memory();
void fetch_next_op();
void decode_op();

/* Avoiding f-pointers and giving the switch-statement a cleaner look */
void case_0();
void case_8();
void case_E();
void case_F();
/* void emulate_cycle(); */


