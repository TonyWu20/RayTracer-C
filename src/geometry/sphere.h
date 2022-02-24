#pragma once
#include <geometry/shape.h>
#include <geometry/type_common.h>
#include <materials/materials.h>
#include <ray/ray.h>
#include <stdio.h>
#include <types/types.h>

/* struct Sphere */
/* { */
/*     simd_float4 origin; // (x, y, z, w) */
/*     float r;            // radius */
/*     simd_float4x4 transform; */
/*     Material m; */
/*     simd_float4 (*surface_normal_at)(const Sphere *self, */
/*                                      simd_float4 *worldPoint); */
/*     void (*set_transform)(Sphere *self, simd_float4x4 *transformMat); */
/* }; */
struct Sphere
{
    Shape *shape;
    Point origin;
    struct Shape_funcTab *funcTab;
    float r;
};

/* @abstract: Create a struct resembling a Sphere */
FUNC Sphere *create_Sphere(Point origin, float r);
/* @abstract: Default unit sphere at origin */
FUNC Sphere *create_Sphere(void);
/* @abstract: Find intersect with ray */
/* @params: void *selfPtr (to be casted into Sphere * in function),
 * Matrix_4x4 *transformimat */
void Sphere_intersect_with_ray(void *selfPtr, Ray *r);
/* @abstract: Setup a transformation to the Sphere */
/* @params: Sphere *self, simd_float4x4 *transformMat */
inline void Sphere_set_transform(void *selfPtr, Matrix_4x4 *transformMatPtr);
/* @abstract: Return a surface normal for a point at surface of the Sphere
 */
Vector Sphere_surface_normal_at(const void *self, Point *worldPoint);
inline void Sphere_destroy(void *selfPtr);
#pragma mark -Implementations

/* static inline Sphere create_Sphere(simd_float4 origin, float r) */
/* { */
/*     Sphere result; */
/*     result.origin = origin; */
/*     result.r = r; */
/*     result.m = defaultMaterial(); */
/*     result.transform = matrix_identity_float4x4; */
/*     result.surface_normal_at = surface_normal_at; */
/*     result.set_transform = set_transform; */
/*     return result; */
/* } */
/* static inline simd_float4 surface_normal_at(const Sphere *self, */
/*                                             simd_float4 *worldPoint) */
/* { */
/*     if (worldPoint->w != 1) */
/*     { */
/*         printf("Not a valid point\n"); */
/*         return (simd_float4){0, 0, 0, 0}; */
/*     } */
/*     simd_float4 objectPoint = */
/*         simd_mul(simd_inverse(self->transform), *worldPoint); */
/*     simd_float4 objectNormal = objectPoint - self->origin; */
/*     simd_float4 worldNormal = */
/*         simd_mul(simd_transpose((simd_inverse(self->transform))),
 * objectNormal); */
/*     worldNormal.w = 0; */
/*     return simd_normalize(worldNormal); */
/* } */
/*  */
/* static inline void set_transform(Sphere *self, simd_float4x4
 * *transformMatPtr) */
/* { */
/*     self->transform = simd_mul(*transformMatPtr, self->transform); */
/* } */
