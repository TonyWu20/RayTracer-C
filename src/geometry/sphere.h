#pragma once
#include <materials/materials.h>
#include <simd/simd.h>
#include <stdio.h>
#define EPSILON 0.0001

typedef struct
{
    simd_float3 origin; // (x, y, w)
    float r;            // radius
    simd_float3x3 transform;
} Circle;

typedef struct Sphere Sphere;
struct Sphere
{
    simd_float4 origin; // (x, y, z, w)
    float r;            // radius
    simd_float4x4 transform;
    Material m;
    simd_float4 (*surface_normal_at)(const Sphere *self,
                                     simd_float4 *worldPoint);
    void (*set_transform)(Sphere *self, simd_float4x4 *transformMat);
};

/* @abstract: Create a struct resembling a Circle */
static inline Circle create_Circle(simd_float3 origin, float r);
/* @abstract: Create a struct resembling a Sphere */
static inline Sphere create_Sphere(simd_float4 origin, float r);
/* @abstract: Return a surface normal for a point at surface of the Sphere
 */
static inline simd_float4 surface_normal_at(const Sphere *self,
                                            simd_float4 *worldPoint);
/* @abstract: Setup a transformation to the Sphere
 * @params: Sphere *self, simd_float4x4 *transformMat
 * @returns: void
 */
static inline void set_transform(Sphere *self, simd_float4x4 *transformMatPtr);

#pragma mark -Implementations
static inline Circle create_Circle(simd_float3 origin, float r)
{
    Circle result;
    result.origin = origin;
    result.r = r;
    result.transform = matrix_identity_float3x3;
    return result;
}

static inline Sphere create_Sphere(simd_float4 origin, float r)
{
    Sphere result;
    result.origin = origin;
    result.r = r;
    result.m = defaultMaterial();
    result.transform = matrix_identity_float4x4;
    result.surface_normal_at = surface_normal_at;
    result.set_transform = set_transform;
    return result;
}
static inline simd_float4 surface_normal_at(const Sphere *self,
                                            simd_float4 *worldPoint)
{
    if (worldPoint->w != 1)
    {
        printf("Not a valid point\n");
        return (simd_float4){0, 0, 0, 0};
    }
    simd_float4 objectPoint =
        simd_mul(simd_inverse(self->transform), *worldPoint);
    simd_float4 objectNormal = objectPoint - self->origin;
    simd_float4 worldNormal =
        simd_mul(simd_transpose((simd_inverse(self->transform))), objectNormal);
    worldNormal.w = 0;
    return simd_normalize(worldNormal);
}

static inline void set_transform(Sphere *self, simd_float4x4 *transformMatPtr)
{
    self->transform = simd_mul(*transformMatPtr, self->transform);
}
