#include "chip8.h"
#include "disp.h"
#include <stdio.h>
#include <string.h>


// TODO: add sdl2 support

int main(int argc, char *argv[]){
  int d;

  for(int i = 0; i < argc; i++){
    if(strcmp("-d", argv[i]) == 0){
      debug = 1;
      break;
    }
    else
      debug = DEBUG;
  }

  init_window();
  start: init();
  d = 0;

  if(argc > 1)
    load_game(argv[1]);
  else
    load_game(TEST6);

  print_memory();

  while(1){
    /* for(int i=0; i<500000; i++){} // Change this to some kind of delay */
    if(tick()){
      fetch_next_op();
      decode_op();
      update_timer();
    }

    if(handle_event())
      break;

    if(debug && d == 0){
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
    else if(debug)
      d--;
  }
}
