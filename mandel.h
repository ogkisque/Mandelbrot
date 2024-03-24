#ifndef MANDEL_H
#define MANDEL_H

#include <stdio.h>
#include "graphics.h"

const char* const   header      = "Мандельброт";
const int           size_x      = 600;
const int           size_y      = 600;
const float         r2_max      = 100;
const int           max_cycles  = 256;

void get_pixels     (uint8_t** pixels);

#endif // MANDEL_H