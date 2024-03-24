#include "graphics.h"

SDL_Window*     window      = NULL;
SDL_Renderer*   renderer    = NULL;
SDL_Texture*    texture     = NULL;  

void init_graphic (const char* header, int size_x, int size_y)
{
    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        printf ("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow (header, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          size_x, size_y, 0);
    if (!window)
    {
        printf ("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf ("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
    SDL_RenderClear (renderer);
    SDL_RenderPresent (renderer);

    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_ARGB8888, 
                                SDL_TEXTUREACCESS_STREAMING, 
                                size_y, 
                                size_x);
}

void draw_pixel (int x, int y, int r_col, int g_col, int b_col)
{
    SDL_SetRenderDrawColor (renderer, r_col, g_col, b_col, 255);
    SDL_RenderDrawPoint (renderer, x, y);
    SDL_SetRenderDrawColor (renderer, 0, 0, 0, 0);
    SDL_RenderPresent (renderer);
}

void lock_texture (void** pixels)
{
    int pitch = 600 * 600 * 4;
    SDL_LockTexture(texture, NULL, pixels, &pitch);
}

void render_texture ()
{
    SDL_UnlockTexture (texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void quit_graphic ()
{
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit ();
}

