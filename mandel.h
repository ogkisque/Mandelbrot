#ifndef MANDEL_H
#define MANDEL_H

#include <stdio.h>
#include <immintrin.h>
#include "graphics.h"

//#define AVX
#define CHECK_TICKS

const char* const   HEADER          = "Мандельброт";
const int           SIZE_X          = 1200;
const int           SIZE_Y          = 1200;
const float         R2_MAX          = 100;
const int           MAX_CYCLES      = 256;
const float         DEF_SCALE       = 2.2f;
const float         DEF_OFFSET_X    = 50.0f;
const float         DEF_OFFSET_Y    = 0.0f;
const int           NUM_TICK_TESTS  = 100;

inline void update_pixels       (uint8_t** pixels, float scale, float offset_x, float offset_y);
inline void update_pixels_avx   (uint8_t** pixels, float scale, float offset_x, float offset_y);
void        print_mandel        (uint8_t** pixels);
inline void fill_pixel          (uint8_t** pixels, int i, int pixel_index);
inline void check_ticks         (uint8_t** pixels);

#endif // MANDEL_H