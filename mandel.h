#ifndef MANDEL_H
#define MANDEL_H

#include <stdio.h>
#include <immintrin.h>
#include "graphics.h"

#define AVX

const char* const   header          = "Мандельброт";
const int           size_x          = 800;
const int           size_y          = 800;
const float         r2_max          = 100;
const int           max_cycles      = 256;
const float         def_scale       = 2.2f;
const float         def_offset_x    = 50.0f;
const float         def_offset_y    = 0.0f;

static inline void update_pixels       (uint8_t** pixels, float scale, float offset_x, float offset_y);
static inline void update_pixels_avx   (uint8_t** pixels, float scale, float offset_x, float offset_y);
void               print_mandel        (uint8_t** pixels);
static inline void fill_pixel          (uint8_t** pixels, int i, int pixel_index);

#endif // MANDEL_H