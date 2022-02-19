#include "cannon.h"

struct projectile tick(struct environment *env, struct projectile *proj)
{
    struct projectile new;
    new.position = proj->position + proj->velocity;
    new.velocity = proj->velocity + env->gravity + env->wind;
    return new;
}
struct projectile init_projectile(double *pos, double *velocity)
{
    struct projectile new;
    simd_double3 pos_vec = simd_make_double3(pos[0], pos[1], pos[2]);
    simd_double3 velocity_vec = simd_make_double3(
        simd_normalize(simd_make_double2(velocity[0], velocity[1])), 0);
    new.position = pos_vec;
    new.velocity = velocity_vec;
    return new;
}
struct environment init_env(double *gravity, double *wind)
{
    struct environment new;
    new.gravity = simd_make_double3(gravity[0], gravity[1], gravity[2]);
    new.wind = simd_make_double3(wind[0], wind[1], wind[2]);
    return new;
}
