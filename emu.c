#include "chip8.h"
#include <stdio.h>

int main(){

  int d;

  start: init();
  load_game();
  print_memory();
  d = 0;

  while(1){

    fetch_next_op();
    decode_op();

    if(DEBUG && d == 0){
      print_pixel();
      while(1){
        printf(">");
        scanf("%d", &d);
        if(d == 999){
          goto start;
        }
        if(d > 0){
          d--;
          break;
        }
      }
    }
    else if(DEBUG)
      d--;
  }


/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
/*   decode_op(); */
/*   fetch_next_op(); */
}
