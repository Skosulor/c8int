#ifndef DISP_H
#define DISP_H
#include <stdio.h>
#include "SDL.h"
#include "chip8.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 400

#define C8_MAX_WIDTH  64
#define C8_MAX_HEIGHT 32
#define SCALE_TO_W   800
#define SCALE_TO_H   400

typedef uint8_t bool;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

int x_scale_factor;
int y_scale_factor;

bool w_set;
bool h_set;
int window_width;
int window_height;

void set_h_pixels(char s[]);
void set_w_pixels(char s[]);
int handle_event();
void init_window();
void test_disp();
void test_window();
void clean_display();
void draw_display();
void destroy_window();
#endif
