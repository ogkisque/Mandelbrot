#include "mandel.h"

void get_pixels (uint8_t** pixels)
{
    lock_texture ((void**) pixels);

    int x = 0;
    int y = 0;
    float dx = 1 / (float) size_x;
    float dy = 1 / (float) size_y;
    float scale = 2.2;
    float offset_x = 50;
    float offset_y = 0;
    int j = 0;

    for (y = 0; y < size_y; y++)
    {
        float y0 = ((float) y - (float) size_y / 2 - offset_y) * dy * scale;

        for (x = 0; x < size_x; x++)
        {
            float x0 = ((float) x - (float) size_x / 2 - offset_x) * dx * scale;
            float tmp_x = x0;
            float tmp_y = y0;
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
                *(*pixels + j) = i % 170 + 20;
                *(*pixels + j + 1) = i % 40 + 20;
                *(*pixels + j + 2) = i % 30 + 30;
            }
            else
            {
                if (i % 2 == 1)
                {
                    *(*pixels + j) = i % 20 + 20;
                    *(*pixels + j + 1) = i % 170 + 20;
                    *(*pixels + j + 2) = i % 30 + 10;
                }
                else
                {
                    *(*pixels + j) = i % 170 + 20;
                    *(*pixels + j + 1) = i % 30 + 20;
                    *(*pixels + j + 2) = i % 20 + 20;
                }
            }

            j += 4;
        }
    }

    render_texture ();
}