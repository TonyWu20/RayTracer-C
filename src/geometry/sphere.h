#pragma once
#include <geometry/shape.h>
#include <materials/materials.h>
#include <ray/ray.h>
#include <stdio.h>
#include <types/common.h>
#include <types/types.h>

struct Sphere
{
    Shape shape;
    Point origin;
    float r;
};

/* @abstract: Create a struct resembling a Sphere */
FUNC Sphere *create_Sphere(Point origin, float r);
/* @abstract: Default unit sphere at origin */
FUNC Sphere *create_Sphere(void);
/* @abstract: Find intersect with ray */
/* @params: void *selfPtr (to be casted into Sphere * in function),
 * Matrix_4x4 *transformimat */
void Sphere_intersect_with_ray(Shape *selfPtr, Ray *r);
/* @abstract: Setup a transformation to the Sphere */
/* @params: Sphere *self, simd_float4x4 *transformMat */
void Sphere_set_transform(Shape *selfPtr, Matrix_4x4 *transformMatPtr);
/* @abstract: Return a surface normal for a point at surface of the Sphere
 */
Vector Sphere_surface_normal_at(const Shape *self, Point *worldPoint);
void Sphere_destroy(Shape *selfPtr);
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
