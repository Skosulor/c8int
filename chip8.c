#include <stdlib.h>
#include <stdio.h>
#include "chip8.h"

void load_game(){
  uint16_t mem_ptr = 0;
  FILE *fptr;
  char game_file[10] = "test";
  char ch;

  fptr = fopen(game_file, "rb");
  if(fptr==NULL){
    printf("No such game");
    exit(0);
  }

  size_t n_bytes;
  n_bytes = fread(memory + 0x200, sizeof(uint8_t), RAM_SIZE, fptr);

  printf("%d Instructions loaded into memory\n", n_bytes);
  fclose(fptr);
}

void init(){
  int i;
  // TODO: dont zero 0-1FF
  for(i = 0; i < RAM_SIZE; i++){
    memory[i] = 0;
  }
  pc = PROGRAM_START;
  i_reg = 0;
  op_code = 0;
  sp = 0;
  // TODO: load fontset
}

void print_memory(){
  int i;
  op_code = 0xFFFF;
  while(op_code != 0000){
    fetch_next_op();
    /* printf("%x: %X\n", pc-2, op_code); */
  }
  pc = 0x200;
}

void fetch_next_op(){
  op_code = (memory[pc] << 8) | memory[pc+1];
  printf("pc: %x, op: %X\n", pc, op_code);
  pc += 2;
}

void case_0(){
  switch(op_code & 0x00FF)
    {
    case 0x00E0:
      // clear screen
      break;
    case 0x00EE:
      // return from subroutine
      break;

    default:
      printf("Error, uknown op_code: %X, case_0\n" , op_code);
  }
}
void case_8(){
}
void case_E(){
}
void case_F(){
}

void decode_op(){
  uint8_t  x   = GET_X(op_code);
  uint8_t  y   = GET_Y(op_code);
  uint8_t  n   = GET_N(op_code);
  uint8_t  nn  = GET_NN(op_code);
  uint16_t nnn = GET_NNN(op_code);

  switch(op_code & 0xF000)
    {
    case 0x0000:
      // 3 cases call new func and switch
      case_0();
      break;

    case 0x1000:
      // jump to adress NNN
      pc = nnn;
      break;

    case 0x2000:
      // call subroutine at NNN
      stack[sp] = pc;
      sp++;
      pc = nnn;
      break;

    case 0x3000:
      // skip next inst if Vx == NN
      if(V[x] == nn){
        pc +=2;
      }
      break;

    case 0x4000:
      // skip next inst if Vx != NN
      if(V[x] != nn){
        pc += 2;
      }
      break;

    case 0x5000:
      // skip inst if Vx == Vy
      if(V[x] == V[y]){
        pc += 2;
      }
      break;

    case 0x6000:
      // set Vx to NN
      V[x] == nn;
      break;

    case 0x7000:
      // adds nn to Vx (carry flag is not changed)
      V[x] +=  nn;
      break;

    case 0x8000:
      // multiple
      case_8();
      break;

    case 0x9000:
      // skips next inst if Vx != Vy
      if(V[x] != V[y]){
        pc+=2;
      }
      break;

    case 0xA000:
      // sets I to NNN
      i_reg = nnn;
      break;

    case 0xB000:
      // jump to address V0 + NNN
      pc = V[0] + nnn;
      break;

    case 0xC000:
      // TODO: Sets Vx to result of (random number & NN)
      break;

    case 0xD000:
      // draw(Vx,Vy,N) TODO
      break;

    case 0xE000:
      // multiple
      case_E();
      break;

    case 0xF000:
      // multiple
      case_F();
      break;

  }
}
