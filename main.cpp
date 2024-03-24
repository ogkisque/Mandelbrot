#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "mandel.h"

int main ()
{
    init_graphic (header, size_x, size_y);

    uint8_t* pixels = (uint8_t*) calloc (size_x * size_y * 4, sizeof (uint8_t));

    get_pixels (&pixels);
    
    SDL_Delay (5000);
    quit_graphic ();
    free (pixels);
    return 0;
}

