#ifndef GRAPH_H
#define GRAPH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void init_graphic     (const char* header, int size_x, int size_y);
void quit_graphic     ();
void draw_pixel       (int x, int y, uint8_t r_col, uint8_t g_col, uint8_t b_col);
void lock_texture     (void** pixels);
void render_texture   ();
void print_text       (const char* text);

const char* const FONT_TTF_FILE = "sans.ttf";

#endif // GRAPH_H