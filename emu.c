#include "chip8.h"
#include <stdio.h>

int main(){

  init();
  load_game();
  print_memory();

  while(1){

    fetch_next_op();
    decode_op();
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
