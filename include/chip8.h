#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "disp.h"

#define FALSE 0
#define TRUE  1

#define RAM_SIZE       0xFFF
#define PROGRAM_START  0x200
#define W_PIXELS       64
#define H_PIXELS       32
#define FONTSET_OFFSET 0x50
#define FONTSET_SIZE   80
#define NEW_PAGE       120

#define DEBUG 0

/* Instruction masking
   0XYN
   0XNN
   0NNN */
#define GET_NNN(X) ((X & 0x0FFF))
#define GET_NN(X)  ((X & 0x00FF))
#define GET_N(X)   ((X & 0x000F))
#define GET_X(X)   ((X & 0x0F00) >> 8)
#define GET_Y(X)   ((X & 0x00F0) >> 4)
#define CF(F)      if(F) V[0xF] = 1; else V[0xF] = 0;

#define TEST1 "test3"
#define TEST2 "test_opcode.ch8"
#define TEST3 "PONG"
#define TEST4 "zerodemo"
#define TEST5 "pdemo"
#define TEST6 "trip"

typedef uint8_t bool;


/* emulated ram memory */
uint8_t debug;
uint8_t memory[RAM_SIZE];
/* 2048 pixels in total */
bool pixel[H_PIXELS][W_PIXELS];
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


static const uint8_t c8_fontset[80] ={
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void init();
void load_game(char rom[]);
void print_memory();
void fetch_next_op();
void decode_op();
void clear_display();
void store_regs(uint8_t to_reg);
void read_regs(uint8_t to_reg);
void draw_pixels(uint8_t x, uint8_t y, uint8_t n);
void print_pixel();
void update_timer();
int tick();

/* Avoiding f-pointers and giving the switch-statement a cleaner look */
void case_0();
void case_8(uint8_t x, uint8_t y, uint8_t n);
void case_E(uint8_t x, uint8_t nn);
void case_F(uint8_t x, uint8_t nn);
/* void emulate_cycle(); */



#endif
