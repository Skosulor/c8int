#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include "disp.h"
#include "chip8.h"

void load_game(char rom[]){
  uint16_t mem_ptr = 0;
  FILE *fptr;
  char ch;

  fptr = fopen(rom, "rb");
  if(fptr==NULL){
    printf("%s\n", rom);
    printf("No such game\n");
    exit(0);
  }

  n_bytes = fread(memory + 0x200, sizeof(uint8_t), RAM_SIZE, fptr);

  printf("%zu Instructions loaded into memory\n", n_bytes/2);
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
  for(i = 0; i < FONTSET_SIZE; i++){
    memory[i + FONTSET_OFFSET] = c8_fontset[i];
  }
  srandom(time(NULL));
  clear_display();
  for(i=0; i<16; i++){
    V[i] = 0;
    keypad[i] = 0;
  }
  if(!update_period_set)
    update_period_us = HZ_714;
}

void print_memory(){
  int i;
  op_code = 0xFFFF;
  printf("--- Loaded Instructions ---\n");
  for(i=0; i<n_bytes/2;i++){
    fetch_next_op();
    printf("%x: %X\n", pc-2, op_code);
  }
  printf("--- End of Instructions ---\n\n");
  pc = 0x200;
}

void fetch_next_op(){
  op_code = (memory[pc] << 8) | memory[pc+1];
  pc += 2;
}

void clear_display(){
  int i,j;
  for(i=0; i<H_PIXELS; i++){
    for(j=0; j<W_PIXELS; j++){
      pixel[i][j] = FALSE;
    }
  }
}

void set_update_freq(char s[]){
  int hz = atoi(s);
  update_period_us = 1000000/hz;
  update_period_set = TRUE;
}

int tick(){
  static bool first = TRUE;
  uint64_t delta_us;
  static struct timespec last_tick, current_time;

  if(first){
    clock_gettime(CLOCK_MONOTONIC_RAW, &last_tick);
    first = FALSE;
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &current_time);
  delta_us = (current_time.tv_sec - last_tick.tv_sec) * 1000000 + (current_time.tv_nsec - last_tick.tv_nsec) / 1000;
  if(delta_us >= update_period_us){
    clock_gettime(CLOCK_MONOTONIC_RAW, &last_tick);
    return 1;
  }
  usleep(20);
  return 0;


}
void update_timer(){
  uint64_t delta_us;
  static struct timespec last, current;
  if(delay_timer == 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &last);
  }
  else if(delay_timer > 0){
    clock_gettime(CLOCK_MONOTONIC_RAW, &current);
    delta_us = (current.tv_sec - last.tv_sec) * 1000000 + (current.tv_nsec - last.tv_nsec) / 1000;

    if(delta_us > 16666){
      delay_timer -= 1;
      sound_timer = 0;
      clock_gettime(CLOCK_MONOTONIC_RAW, &last);
    }
  }
  else{
    printf("Error updating delay timer, timer value = x%X\n", delay_timer);
    while(1){}
  }
}

void case_0(){
  switch(op_code & 0x00FF)
    {
    case 0x00E0: clear_display(); break;
    case 0x00EE:
      if(sp <= 0){printf("Error: Stackpointer < 0");}
      sp--;
      pc = stack[sp];
      break;

    default:
      printf("Error, uknown op_code: %X, case_0\n" , op_code);
      while(1){
        destroy_window();
      }
    }

}
void case_8(uint8_t x, uint8_t y, uint8_t n){
  switch(n)
    {
    case 0x0: V[x]  = V[y];        break;
    case 0x1: V[x]  = V[x] | V[y]; break;
    case 0x2: V[x]  = V[x] & V[y]; break;
    case 0x3: V[x]  = V[x] ^ V[y]; break;
    case 0x4: CF(V[x]>UINT8_MAX-V[y]); V[x] += V[y];       break;
    case 0x5: CF(V[x]>V[y]);           V[x] -= V[y];       break;
    case 0x6: CF(V[x]&0b00000001);     V[x] = V[x] >> 1;   break;
    case 0x7: CF(V[y]>V[x]);           V[x] = V[y] - V[x]; break;
    case 0xE: CF(V[x]&0b10000000);     V[x] = V[x] << 1;   break;

    default:
      printf("Error, uknown op_code: %X, case_0\n" , op_code);
      while(1){
        destroy_window();
      }
    }
}

void case_E(uint8_t x, uint8_t nn){
  switch(nn){
  case 0x9E:
    if(keypad[V[x]] == TRUE){
      pc +=2;
    }
    break;
  case 0xA1:
    if(keypad[V[x]] != TRUE){
      pc +=2;
    }
    break;
  default:
    printf("Error, uknown op_code: %X, case_0\n" , op_code);
    while(1){
      destroy_window();
    }
  }
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

void wait_keypress(uint8_t x){
  bool brk = FALSE;
  while(1){
    usleep(10);
    handle_event();
    for(int i=0; i<16; i++){
      if(keypad[i] == TRUE){
        V[x] = i;
        brk = TRUE;
        break;
      }
    }
    if(brk)
      break;
  }
}

void case_F(uint8_t x, uint8_t nn){
  switch(nn)
    {
    case 0x07: V[x] = delay_timer;    break;
    case 0x0A: wait_keypress(x);      break;
    case 0x15: delay_timer = V[x];    break;
    case 0x18: sound_timer = V[x];    break;
    case 0x1E: i_reg = i_reg + V[x];  break;
    case 0x29: i_reg = 0x50 + V[x]*5; break;
    case 0x33: get_bcd(x);            break;
    case 0x55: store_regs(x);         break;
    case 0x65: read_regs(x);          break;

    default:
      printf("Error, uknown op_code: %X, case_0\n" , op_code);
      while(1){
        destroy_window();
      }
    }
}

void draw_pixels(uint8_t x, uint8_t y, uint8_t n){
  int i,j;
  bool old_p;

  if(debug){
    printf("Draw pixels at x: %d, y: %d\n", V[x], V[y]);
  }

  V[0xF] = 0;

  for(i = 0; i < n; i ++){
    for(j = 0; j < 8; j++){
      old_p = pixel[V[y]+i][V[x]+j];

      if(((memory[i_reg+i]) & (0x80 >> j)) == 0){
        pixel[V[y]+i][V[x]+j] = FALSE;
        if(old_p == TRUE){
          pixel[V[y]+i][V[x]+j] = TRUE;
        }
      }
      else{
        if(old_p == TRUE){
          pixel[V[y]+i][V[x]+j] = FALSE;
          V[0xF] = 1;
        }
        else
          pixel[V[y]+i][V[x]+j] = TRUE;
      }
    }
  }

  /* print_pixel(); */
  clean_display();
  draw_display();
}

void decode_op(){
  uint8_t  x   = GET_X(op_code);
  uint8_t  y   = GET_Y(op_code);
  uint8_t  n   = GET_N(op_code);
  uint8_t  nn  = GET_NN(op_code);
  uint16_t nnn = GET_NNN(op_code);
  /* printf("pc: %x, op: %x, x: %x, y: %x, nnn: %x, nn: %x, n: %x\n", pc-2, op_code, x, y, nnn, nn, n); */
  switch(op_code & 0xF000)
    {
    case 0x0000: case_0();                       break;
    case 0x1000: pc = nnn;                       break;
    case 0x2000: stack[sp] = pc; sp++; pc = nnn; break;
    case 0x3000: if(V[x] == nn){ pc +=2; }       break;
    case 0x4000: if(V[x] != nn){ pc += 2; }      break;
    case 0x5000: if(V[x] == V[y]){ pc += 2;}     break;
    case 0x6000: V[x] = nn;                      break;
    case 0x7000: V[x] +=  nn;                    break;
    case 0x8000: case_8(x,y,n);                  break;
    case 0x9000: if(V[x] != V[y]){ pc+=2; }      break;
    case 0xA000: i_reg = nnn;                    break;
    case 0xB000: pc = V[0] + nnn;                break;
    case 0xC000: V[x] = (random() % 255) & nn;   break;
    case 0xD000: draw_pixels(x,y,n);             break;
    case 0xE000: case_E(x,nn);                   break;
    case 0xF000: case_F(x,nn);                   break;
    default:
      printf("Error, uknown op_code: %X, main switch\n" , op_code);
      while(1){
        destroy_window();
      }
    }
}

void print_pixel(){

  int i,j,d;
  uint16_t temp_op_code;
  unsigned char block = 127;
  d = -10;
  for(i = 0; i < NEW_PAGE; i++){
    printf("\n");
  }

  for(i = 0; i < H_PIXELS; i++){
    for(j = 0; j < W_PIXELS; j++){

      if(pixel[i][j])
        printf("%c", block);
      else
        printf(" ");
    }

    if(debug){
      temp_op_code = (memory[pc+d] << 8) | memory[pc+d+1];
      if(i <=15)
        printf("  |   V%x: %X", i, V[i]);
      else if(i == 16){
        printf("  |   I reg: %x", i_reg);
      }
      else if(pc+d == pc){
        printf("  |   addr: %x, op: %X <- PC; SP: %d", pc+d,  temp_op_code, sp);
        d+=2;
      }
      else{
        printf("  |   addr: %x, op: %X", pc+d,  temp_op_code);
        d+=2;
      }
    }

    printf("\n");
  }
}

