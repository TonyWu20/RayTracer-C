#include "cannon/cannon.h"
#include "canvas/canvas.h"
#include <simd/simd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FRAMERATE 60
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;

void test_projectile(double vMultipier)
{
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    double position[3] = {0, 0.1, 0};
    double velocity[3] = {1.8, 1.2, 0};
    struct projectile p = init_projectile(position, velocity);
    p.velocity = p.velocity * vMultipier;
    double gravity[3] = {0, -0.1, 0};
    double wind[3] = {-0.01, 0, 0};
    struct environment env = init_env(gravity, wind);
    int tickNum = 0;
    printf("Initial Position: (%f, %f)\n", p.position.x, p.position.y);
    Canvas *fig = init_Canvas(200, 46);
    while (p.position[1] > 0.0)
    {
        p = tick(&env, &p);
        fig->writePixel(fig, (int)p.position.x, (int)p.position.y, '*');
        fig->show(fig);
        nanosleep(&tim, &tim2);
        tickNum++;
    }
    printf("\n%d frame(s) for the projectile to hit the ground!\n", tickNum);
    fig->destroy(fig);
}

int main(int argc, char *argv[])
{
    double vMultipier = 1.0;
    if (argc > 1)
    {
        vMultipier = atof(argv[1]);
    }
    test_projectile(vMultipier);
    return 0;
}
