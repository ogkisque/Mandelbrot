#ifndef GRAPH_H
#define GRAPH_H

#include <SDL.h>
#include <stdio.h>

void init_graphic   (const char* header, int size_x, int size_y);
void quit_graphic   ();
void draw_pixel     (int x, int y, int r_col, int g_col, int b_col);
void lock_texture   (void** pixels);
void render_texture ();

#endif // GRAPH_H