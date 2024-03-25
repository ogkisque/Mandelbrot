#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "mandel.h"

int main ()
{
    init_graphic (HEADER, SIZE_X, SIZE_Y);

    uint8_t* pixels = (uint8_t*) calloc (SIZE_X * SIZE_Y * 4, sizeof (uint8_t));

    print_mandel (&pixels);
    
    quit_graphic ();
    free (pixels);
    return 0;
}

