#pragma once
#include <simd/simd.h>

typedef struct
{
    simd_float3 origin; // (x, y, w)
    float r;            // radius
    simd_float3x3 transform;
} Circle;
typedef struct
{
    simd_float4 origin; // (x, y, z, w)
    float r;            // radius
    simd_float4x4 transform;
} Sphere;

static inline Circle create_Circle(simd_float3 origin, float r);
static inline Sphere create_Sphere(simd_float4 origin, float r);

static inline Circle create_Circle(simd_float3 origin, float r)
{
    Circle result;
    result.origin = origin;
    result.r = r;
    result.transform = simd_diagonal_matrix((simd_float3){1, 1, 1});
    return result;
}

static inline Sphere create_Sphere(simd_float4 origin, float r)
{
    Sphere result;
    result.origin = origin;
    result.r = r;
    result.transform = simd_diagonal_matrix((simd_float4){1, 1, 1, 1});
    return result;
}
