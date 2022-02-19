#include "cannon.h"
#include <simd/simd.h>
#include <stdio.h>
#include <stdlib.h>

void test_projectile(double vMultipier)
{
    double position[3] = {0, 1, 0};
    double velocity[3] = {1, 1, 0};
    struct projectile p = init_projectile(position, velocity);
    p.velocity = p.velocity * vMultipier;
    double gravity[3] = {0, -0.1, 0};
    double wind[3] = {-0.01, 0, 0};
    struct environment env = init_env(gravity, wind);
    int tickNum = 0;
    printf("Initial Position: (%f, %f)\n", p.position.x, p.position.y);
    while (p.position[1] > 0.0)
    {
        p = tick(&env, &p);
        printf("Position: (%f, %f)\n", p.position.x, p.position.y);
        tickNum++;
    }
    printf("%d ticks for the projectile to hit the ground!\n", tickNum);
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
