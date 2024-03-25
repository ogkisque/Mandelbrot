#include "mandel.h"

void print_mandel (uint8_t** pixels)
{
    SDL_Event event;
    bool quit = false;
    float scale = def_scale;
    float offset_x = def_offset_x;
    float offset_y = def_offset_y;

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

        #ifndef AVX
        update_pixels (pixels, scale, offset_x, offset_y);
        #else
        update_pixels_avx (pixels, scale, offset_x, offset_y);
        #endif
    }
}

static inline void update_pixels (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    lock_texture ((void**) pixels);

    int x = 0;
    int y = 0;
    float dx = 1 / (float) size_x;
    float dy = 1 / (float) size_y;
    int pixel_index = 0;

    Uint32 time0 = SDL_GetTicks ();

    for (y = 0; y < size_y; y++)
    {
        float y0 = ((float) y - (float) size_y / 2 - offset_y) * dy * scale;

        for (x = 0; x < size_x; x++)
        {
            float x0 = ((float) x - (float) size_x / 2 - offset_x) * dx * scale;
            int i = 0;

            for (float tmp_x = x0, tmp_y = y0; i < max_cycles; i++)
            {
                float x2 = tmp_x * tmp_x;
                float y2 = tmp_y * tmp_y;
                float xy = tmp_x * tmp_y;

                if (x2 + y2 >= r2_max) break;

                tmp_x = x2 - y2 + x0;
                tmp_y = xy + xy + y0;
            }

            fill_pixel (pixels, i, pixel_index);
            pixel_index += 4;
        }
    }

    Uint32 time1 = SDL_GetTicks ();

    render_texture ();

    Uint32 time2 = SDL_GetTicks ();

    printf ("calculation %u rendering %u fps %f\n", time1 - time0, time2 - time0, 1000 / ((float) time2 - (float) time0));
}

static inline void update_pixels_avx (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    lock_texture ((void**) pixels);

    int x               = 0;
    int y               = 0;
    __m256 r2_max_vec   = _mm256_set1_ps (r2_max);
    int pixel_index     = 0;

    Uint32 time0 = SDL_GetTicks ();

    for (y = 0; y < size_y; y++)
    {
        __m256 y0 = _mm256_set1_ps (((float) y - (float) size_y / 2 - offset_y) / (float) size_y * scale);

        for (x = 0; x < size_x; x += 8)
        {
            __m256 x0           = _mm256_add_ps (_mm256_set_ps (7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f),
                                             _mm256_set1_ps (((float) x - (float) size_x / 2 - offset_x)));
            x0                  = _mm256_mul_ps (x0, _mm256_set1_ps (scale / (float) size_x));
            int i               = 0;
            __m256i cmp_i       = _mm256_set1_epi32 (0);

            for (__m256 tmp_x = x0, tmp_y = y0; i < max_cycles; i++)
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

    Uint32 time1 = SDL_GetTicks ();

    render_texture ();

    Uint32 time2 = SDL_GetTicks ();

    printf ("calculation %u rendering %u fps %f\n", time1 - time0, time2 - time0, 1000 / ((float) time2 - (float) time0));
}

static inline void fill_pixel (uint8_t** pixels, int i, int pixel_index)
{
    uint8_t j = (uint8_t) i;
    if (j == max_cycles)
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