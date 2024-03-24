#include "mandel.h"

void print_mandel       (uint8_t** pixels)
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

        update_pixels (pixels, scale, offset_x, offset_y);

    }
}

void update_pixels (uint8_t** pixels, float scale, float offset_x, float offset_y)
{
    lock_texture ((void**) pixels);

    int x = 0;
    int y = 0;
    float dx = 1 / (float) size_x;
    float dy = 1 / (float) size_y;
    int j = 0;

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

            if (i == max_cycles)
            {
                *(*pixels + j) = (uint8_t) i % 170 + 20;
                *(*pixels + j + 1) = (uint8_t) i % 40 + 20;
                *(*pixels + j + 2) = (uint8_t) i % 30 + 30;
            }
            else
            {
                if (i % 2 == 1)
                {
                    *(*pixels + j) = (uint8_t) i % 20 + 20;
                    *(*pixels + j + 1) = (uint8_t) i % 170 + 20;
                    *(*pixels + j + 2) = (uint8_t) i % 30 + 10;
                }
                else
                {
                    *(*pixels + j) = (uint8_t) i % 170 + 20;
                    *(*pixels + j + 1) = (uint8_t) i % 30 + 20;
                    *(*pixels + j + 2) = (uint8_t) i % 20 + 20;
                }
            }

            j += 4;
        }
    }

    render_texture ();
}