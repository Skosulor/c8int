#include "disp.h"
#include <threads.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

int x_scale_factor;
int y_scale_factor;

bool w_set;
bool h_set;
int window_width;
int window_height;



void init_window(){
  
  if(!w_set){
    x_scale_factor = SCALE_TO_W / C8_MAX_WIDTH;
    window_width = WINDOW_WIDTH;
  }
  if(!h_set){
    y_scale_factor = SCALE_TO_H / C8_MAX_HEIGHT;
    window_height = WINDOW_HEIGHT;
  }


  window = SDL_CreateWindow(
    "SDL2Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_width,
    window_height,
    0
    );

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderPresent(renderer);


}

void set_h_pixels(char s[]){
  window_height = atoi(s);
  y_scale_factor = window_height / C8_MAX_HEIGHT;
  h_set = TRUE;
}
void set_w_pixels(char s[]){
  window_width = atoi(s);
  x_scale_factor = window_width / C8_MAX_WIDTH;
  w_set = TRUE;
}

void draw_display(){

  SDL_Rect rect;
  rect.w = x_scale_factor;
  rect.h = y_scale_factor;

  int i,j,xpos,ypos;
  for(i=0; i<H_PIXELS; i++){
    for(j=0; j<W_PIXELS; j++){
      if(pixel[i][j] == TRUE){
        xpos = j * x_scale_factor;
        ypos = i * y_scale_factor;
        rect.x = xpos;
        rect.y = ypos;

        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

void clean_display(){

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

}

int handle_event(){
  bool down;

  SDL_PollEvent(&e);
  down = FALSE;
  if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP){
    if(e.type == SDL_KEYDOWN)
      down = TRUE;

    switch(e.key.keysym.sym){

    case SDLK_q:
      printf("quitting\n");
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 1;
      break;


    case SDLK_KP_0: keypad[0]  = down; break;
    case SDLK_KP_1: keypad[1]  = down; break;
    case SDLK_KP_2: keypad[2]  = down; break;
    case SDLK_KP_3: keypad[3]  = down; break;
    case SDLK_KP_4: keypad[4]  = down; break;
    case SDLK_KP_5: keypad[5]  = down; break;
    case SDLK_KP_6: keypad[6]  = down; break;
    case SDLK_KP_7: keypad[7]  = down; break;
    case SDLK_KP_8: keypad[8]  = down; break;
    case SDLK_KP_9: keypad[9]  = down; break;
    case SDLK_a:    keypad[10] = down; break;
    case SDLK_s:    keypad[11] = down; break;
    case SDLK_d:    keypad[12] = down; break;
    case SDLK_f:    keypad[13] = down; break;
    case SDLK_g:    keypad[14] = down; break;
    case SDLK_h:    keypad[15] = down; break;
    }
    return 0;
  }
  else
    return 0;
}
void destroy_window(){
  SDL_DestroyWindow(window);
  SDL_Quit();
}
