#pragma once
#include <simd/simd.h>

struct environment
{
    simd_double3 gravity;
    simd_double3 wind;
};

struct projectile
{
    simd_double3 position;
    simd_double3 velocity;
};

struct projectile tick(struct environment *env, struct projectile *proj);
struct projectile init_projectile(double pos[3], double velocity[3]);
struct environment init_env(double gravity[3], double wind[3]);
