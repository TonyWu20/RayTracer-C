#include <canvas/canvas.h>
/* #include <ncurses.h> */
#include <geometry/geometry.h>
#include <ray/ray.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define FRAMERATE 60
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;
static inline void castRayOnSphere(float half, float wall_z, float pixel_size,
                                   simd_float4 ray_origin, Sphere *s,
                                   Canvas *fig)
{
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    int shift_x = (fig->width - 2 * fig->height) / 2;
    for (int y = 0; y < fig->height; ++y)
    {
        for (int x = 0; x < fig->width; ++x)
        {
            float world_y = half - pixel_size * y;
            float world_x = -half + pixel_size * x;
            simd_float4 pos = {world_x, world_y, wall_z, 1};
            Ray r = init_Ray(ray_origin, simd_normalize(pos - ray_origin));
            r = r.transform(&r, &s->transform);
            r.intersects_with_Sphere(&r, s);
            IntersectCollection *xs = r.intxnSphere;
            if (xs)
            {
                fig->writePixel(fig, shift_x + 2 * x, y, 'x');
                fig->writePixel(fig, shift_x + 2 * x - 1, y, '%');
                r.intxnSphere->destroy(r.intxnSphere);
            }
        }
        nanosleep(&tim, &tim2);
        fig->show(fig);
    }
}
void silhouette()
{
    float r = 1;
    Sphere s = create_Sphere((simd_float4){0, 0, 0, 1}, r);
    simd_float4 ray_origin = {0, 0, -6, 1};
    float wall_z = 10;
    float on_wall_d = 2 * r * (wall_z - ray_origin.z) / (0.0 - ray_origin.z);
    float wall_size = on_wall_d + 4;
    /* WINDOW *win = initscr(); */
    /* int row, col; */
    /* getmaxyx(win, row, col); */
    int row = 40;
    int col = 200;
    float half = wall_size / 2;
    float pixel_size = wall_size / row;
    printf("\e[?25l");
    Canvas *fig = init_Canvas(col, row);
    castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, fig);
    fig->show(fig);
    sleep(1);
    fig->clear(fig);
    s.transform = scaling_matrix((simd_float3){-0.3, 0.5, 3});
    castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, fig);
    sleep(1);
    fig->clear(fig);
    s.transform =
        simd_mul(shearing_matrix(.5, .2, .2, .2, 1, 0.5), s.transform);
    castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, fig);
    sleep(1);
    fig->clear(fig);
    s.transform =
        simd_mul(translation_matrix((simd_float3){-1, 1.5, 3}), s.transform);
    castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, fig);
    fig->show(fig);
    getchar();
    fig->destroy(fig);
    printf("\e[?25h");
    /* refresh(); */
    /* getch(); */
    /* endwin(); */
    /* delwin(win); */
}

int main(int argc, char *argv[])
{
    silhouette();
    return 0;
}
