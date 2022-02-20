#include "canvas/canvas.h"
#include <simd/simd.h>
#include <time.h>
#define PI (atan(1) * 4)

#define FRAMERATE 24
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;
void drawClock()
{
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    Canvas *fig = init_Canvas(178, 48);
    int origin_x = fig->width / 2;
    int origin_y = fig->height / 2;
    float radius = (float)fig->height * 3 / 8;
    float angle = PI / 6;
    simd_float3 clockPoint = {0, radius, 0};
    for (int i = 0; i < 12; ++i)
    {
        simd_quatf rotation = simd_quaternion(
            angle * i, simd_normalize(simd_make_float3(0, 0, 1)));
        simd_float3 rotatedPt = simd_act(rotation, clockPoint);
        int x, y;
        x = (int)(rotatedPt.x + origin_x);
        y = (int)(rotatedPt.y / 2 + origin_y);
        fig->writePixel(fig, x, y, '*');
    }
    for (int i = 0; i < 720; ++i)
    {
        for (int k = 0; k < (int)(radius * 2 / 3); ++k)
        {
            fig->writePixel(
                fig, origin_x + 2.0 / 3.0 * k * sin((angle * i) / 60),
                (origin_y + 2.0 / 3.0 * k * cos((angle * i) / 60) / 2), '@');
        }
        for (int k = 0; k < (int)(radius * 2 / 3); ++k)
        {
            fig->writePixel(fig, origin_x + k * sin((angle * i)),
                            (origin_y + k * cos((angle * i)) / 2), '#');
        }
        nanosleep(&tim, &tim2);
        fig->show(fig);
        for (int k = 0; k < (int)(radius * 2 / 3); ++k)
        {
            fig->writePixel(fig, origin_x + k * sin((angle * i)),
                            (origin_y + k * cos((angle * i)) / 2), ' ');
        }
        for (int k = 0; k < (int)(radius * 2 / 3); ++k)
        {
            fig->writePixel(
                fig, origin_x + 2.0 / 3.0 * k * sin((angle * i) / 60),
                (origin_y + 2.0 / 3.0 * k * cos((angle * i) / 60) / 2), ' ');
        }
    }
    fig->show(fig);
    fig->destroy(fig);
    return;
}

int main(int argc, char *argv[])
{
    drawClock();
    return 0;
}
