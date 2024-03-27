#include "mandel.h"

void print_mandel (uint8_t** pixels)
{
    SDL_Event event;
    bool quit       = false;
    float scale     = DEF_SCALE;
    float offset_x  = DEF_OFFSET_X;
    float offset_y  = DEF_OFFSET_Y;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case (SDLK_UP):
                        offset_y -= 5;
                        break;
                    case (SDLK_DOWN):
                        offset_y += 5;
                        break;
                    case (SDLK_LEFT):
                        offset_x -= 5;
                        break;
                    case (SDLK_RIGHT):
                        offset_x += 5;
                        break;
                    default:
                        break;
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                int amount {event.wheel.y};
                scale -= (float) amount / 8;
            }
        }

        #ifdef CHECK_TICKS
            check_ticks (pixels);
        #else
            #ifdef AVX
                update_pixels_avx (pixels, scale, offset_x, offset_y);
            #else
                update_pixels (pixels, scale, offset_x, offset_y);
            #endif // AVX
        #endif // CHECK_TICKS
        
    }
}

inline void update_pixels (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    Uint32 time0 = SDL_GetTicks ();

    lock_texture ((void**) pixels);
    calc_pixels (pixels, scale, offset_x, offset_y);
    render_texture ();

    Uint32 time1 = SDL_GetTicks ();

    char fps[10] = "";
    sprintf (fps, "FPS: %d", (int) (1000 / ((float) time1 - (float) time0)));
    print_text (fps);
}

inline void calc_pixels (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    int     x = 0;
    int     y = 0;
    float   dx = 1 / (float) SIZE_X;
    float   dy = 1 / (float) SIZE_Y;
    int     pixel_index = 0;

    for (y = 0; y < SIZE_Y; y++)
    {
        float y0 = (- (float) y - (float) SIZE_Y / 2 - offset_y) * dy * scale;

        for (x = 0; x < SIZE_X; x++)
        {
            float x0 = ((float) x - (float) SIZE_X / 2 - offset_x) * dx * scale;
            int i = 0;

            for (float tmp_x = x0, tmp_y = y0; i < MAX_CYCLES; i++)
            {
                float x2 = tmp_x * tmp_x;
                float y2 = tmp_y * tmp_y;
                float xy = tmp_x * tmp_y;

                if (x2 + y2 >= R2_MAX) break;

                tmp_x = x2 - y2 + x0;
                tmp_y = xy + xy + y0;
            }

            fill_pixel (pixels, i, pixel_index);
            pixel_index += 4;
        }
    }
}

inline void update_pixels_avx (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    Uint32 time0 = SDL_GetTicks ();

    lock_texture ((void**) pixels);
    calc_pixels_avx (pixels, scale, offset_x, offset_y);
    render_texture ();

    Uint32 time1 = SDL_GetTicks ();

    char fps[10] = "";
    sprintf (fps, "FPS: %d", (int) (1000 / ((float) time1 - (float) time0)));
    print_text (fps);
}

inline void calc_pixels_avx (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    int     x               = 0;
    int     y               = 0;
    int     pixel_index     = 0;
    __m256  r2_max_vec      = _mm256_set1_ps (R2_MAX);

    for (y = 0; y < SIZE_Y; y++)
    {
        __m256 y0 = _mm256_set1_ps (((float) y - (float) SIZE_Y / 2 - offset_y) / (float) SIZE_Y * scale);

        for (x = 0; x < SIZE_X; x += 8)
        {
            __m256  x0          = _mm256_add_ps (_mm256_set_ps (7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f),
                                                 _mm256_set1_ps (((float) x - (float) SIZE_X / 2 - offset_x)));

                    x0          = _mm256_mul_ps (x0, _mm256_set1_ps (scale / (float) SIZE_X));
            __m256i cmp_i       = _mm256_set1_epi32 (0);
            int     i           = 0;

            for (__m256 tmp_x = x0, tmp_y = y0; i < MAX_CYCLES; i++)
            {
                __m256 x2   = _mm256_mul_ps (tmp_x, tmp_x);
                __m256 y2   = _mm256_mul_ps (tmp_y, tmp_y);
                __m256 xy   = _mm256_mul_ps (tmp_x, tmp_y);

                __m256 r2   = _mm256_add_ps (x2, y2);
                __m256 cmp  = _mm256_cmp_ps (r2_max_vec, r2, _CMP_GE_OQ);
                cmp_i       = _mm256_sub_epi32 (cmp_i, _mm256_castps_si256 (cmp));

                if (!_mm256_movemask_ps (cmp)) break;

                tmp_x       = _mm256_add_ps (_mm256_sub_ps (x2, y2), x0);
                tmp_y       = _mm256_add_ps (_mm256_add_ps (xy, xy), y0);
            }

            uint32_t* new_cmp_i = (uint32_t*) &cmp_i;
            for (int ofs = 0; ofs < 8; ofs++)
            {
                fill_pixel (pixels, (int) new_cmp_i[ofs], pixel_index);
                pixel_index += 4;
            }
        }
    }
}

inline void fill_pixel (uint8_t** pixels, int i, int pixel_index)
{
    uint8_t j = (uint8_t) i;
    if (j == MAX_CYCLES)
    {
        *(*pixels + pixel_index)     = j % 150;
        *(*pixels + pixel_index + 1) = j % 40 + 20;
        *(*pixels + pixel_index + 2) = j % 30;
    }
    else
    {
        if (j % 2 == 1)
        {
            *(*pixels + pixel_index)     = j % 20;
            *(*pixels + pixel_index + 1) = j % 150 + 20;
            *(*pixels + pixel_index + 2) = j % 30;
        }
        else
        {
            *(*pixels + pixel_index)     = j % 150;
            *(*pixels + pixel_index + 1) = j % 30;
            *(*pixels + pixel_index + 2) = j % 20;
        }
    }
}

inline void check_ticks (uint8_t** pixels)
{
    unsigned long long tick0 = __rdtsc ();

    #ifdef AVX
    calc_pixels_avx (pixels, DEF_SCALE, DEF_OFFSET_X, DEF_OFFSET_Y);
    #else
    calc_pixels (pixels, DEF_SCALE, DEF_OFFSET_X, DEF_OFFSET_Y);
    #endif

    unsigned long long tick1 = __rdtsc ();
    printf ("%llu\n", (tick1 - tick0) / NUM_TICK_TESTS);
}

