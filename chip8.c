#include <stdlib.h>
#include <time.h>
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

  printf("%d Instructions loaded into memory\n", n_bytes/2);
  fclose(fptr);
}

void init(){
  int i;
  for(i = 0; i < RAM_SIZE; i++){
    memory[i] = 0;
  }
  pc = PROGRAM_START;
  i_reg = 0;
  op_code = 0;
  sp = 0;
  // TODO: load fontset
  for(i = 0; i < FONTSET_SIZE; i++){
    memory[i + FONTSET_OFFSET] = c8_fontset[i];
  }
  srandom(time(NULL));
}

void print_memory(){
  int i;
  op_code = 0xFFFF;
  printf("--- Loaded Instructions ---\n");
  while(op_code != 0000){
    fetch_next_op();
    printf("%x: %X\n", pc-2, op_code);
  }
  printf("--- End of Instructions ---\n\n");
  pc = 0x200;
}

void fetch_next_op(){
  op_code = (memory[pc] << 8) | memory[pc+1];
  /* printf("pc: %x, op: %X\n", pc, op_code); */
  pc += 2;
}

void clear_display(){
  int i,j;
  for(i=0; i<W_PIXELS; i++){
    for(j=0; j<H_PIXELS; j++){
      pixel[i][j] = FALSE;
    }
  }
}


void case_0(){
  switch(op_code & 0x00FF)
    {
    case 0x00E0:
      clear_display();
      break;

    case 0x00EE:
      if(sp == 0)
        printf("Error: Stackpointer < 0");
      sp--;
      pc = stack[sp];
      break;

    default:
      printf("Error, uknown op_code: %X, case_0\n" , op_code);
  }

} // DONE
void case_8(uint8_t x, uint8_t y, uint8_t n){     // DONE
  switch(n)
    {
    case 0x0: V[x]  = V[y];        break;
    case 0x1: V[x]  = V[x] | V[y]; break;
    case 0x2: V[x]  = V[x] & V[y]; break;
    case 0x3: V[x]  = V[x] ^ V[y]; break;
    case 0x4: CF(V[x]>UINT8_MAX-V[y]); V[x] += V[y];       break;
    case 0x5: CF(V[x]>V[y]);           V[x] -= V[y];       break;
    case 0x6: CF(V[x]&0b0001);         V[x] = V[x] >> 1;   break;
    case 0x7: CF(V[y]>V[x]);           V[x] = V[y] - V[x]; break;
    case 0xE: CF(V[x]&0b1000);         V[x] = V[x] << 1;   break;
    }
}

void case_E(){     // TODO Rquires input
}

void get_bcd(uint8_t x){
  memory[i_reg]   =  V[x]/100;
  memory[i_reg+1] = (V[x] - memory[i_reg]*100) / 10;
  memory[i_reg+2] =  V[x] - memory[i_reg]*100 - memory[i_reg+1]*10;
}

void store_regs(uint8_t to_reg){
  int i;
  for(i = 0; i <= to_reg; i++){
    memory[i_reg + i] = V[i];
  }
}

void read_regs(uint8_t to_reg){
  int i;
  for(i = 0; i <= to_reg; i++){
    V[i] = memory[i_reg + i];
  }
}

void case_F(uint8_t x, uint8_t nn){
  switch(nn)
    {
    case 0x07: V[x] = delay_timer;   break;
    case 0x0A: break; // TODO wait for keypress, delay all execution
    case 0x15: delay_timer = V[x];   break;
    case 0x18: sound_timer = V[x];   break;
    case 0x1E: i_reg = i_reg + V[x]; break;
    case 0x29: i_reg = 0x50 + x*5;   break; 
    case 0x33: get_bcd(x);    break;
    case 0x55: store_regs(x); break;
    case 0x65: read_regs(x);  break;
    }
}

void decode_op(){
  uint8_t  x   = GET_X(op_code);
  uint8_t  y   = GET_Y(op_code);
  uint8_t  n   = GET_N(op_code);
  uint8_t  nn  = GET_NN(op_code);
  uint16_t nnn = GET_NNN(op_code);

  printf("pc: %x, op: %x, x: %x, y: %x, nnn: %x, nn: %x, n: %x\n", pc, op_code, x, y, nnn, nn, n);

  switch(op_code & 0xF000)
    {
    case 0x0000: case_0();                       break;
    case 0x1000: pc = nnn;                       break;
    case 0x2000: stack[sp] = pc; sp++; pc = nnn; break;
    case 0x3000: if(V[x] == nn){ pc +=2; }       break;
    case 0x4000: if(V[x] != nn){ pc += 2; }      break;
    case 0x5000: if(V[x] == V[y]){ pc += 2;}     break;
    case 0x6000: V[x] == nn;                     break;
    case 0x7000: V[x] +=  nn;                    break;
    case 0x8000: case_8(x,y,n);                  break;
    case 0x9000: if(V[x] != V[y]){ pc+=2; }      break;
    case 0xA000: i_reg = nnn;                    break;
    case 0xB000: pc = V[0] + nnn;                break;
    case 0xC000: V[x] = (random() % 255) & nn;   break;

    case 0xD000:
      // draw(Vx,Vy,N) TODO
      break;

    case 0xE000: case_E(); break;
    case 0xF000: case_F(x,nn); break;

    default:
      printf("Error, uknown op_code: %X, main switch\n" , op_code);
  }
}
