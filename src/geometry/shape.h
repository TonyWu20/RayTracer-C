#pragma once
/* #include <geometry/sphere.h> */
#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED
// clang-format off
#include <materials/materials.h>
#include <types/common.h>
#include <types/types.h>
#include <ray/ray.h>
#include <stdio.h>
// clang-format on
enum
{
    SPHERE,
    PLANE,
} SHAPE_TYPE;

struct Shape
{
    Material *material;
    Matrix_4x4 *transform;
    struct Shape_funcTab *funcTab;
    Ray local_ray;
};

struct Shape_funcTab
{
    void (*intersect_with_ray)(void *shape, Ray *ray);
    void (*set_transform)(void *shape, Matrix_4x4 *transform);
    Vector (*surface_normal_at)(const void *self, Point *worldPoint);
    void (*destroy)(void *self);
};

/* @abstract: Create an abstract super class Shape */
static inline Shape *create_Shape(void);
/* @abstract: Base function for intersect_with_ray */
static inline void Shape_intersect_with_ray(Shape *self, Ray *ray);
/* @abstract: Base function for set_transform */
static inline void Shape_set_transform(Shape *self, Matrix_4x4 *transform);
/* @abstract: Base function for surface_normal_at */
static inline Vector Shape_local_to_world_normal(Shape *self,
                                                 Vector *localNormal);
/* @abstract: Release memory of the Shape struct */
static inline void Shape_destroy(Shape *self);

#pragma mark -Implementation
static inline Shape *create_Shape(void)
{
    Shape *self = malloc(sizeof(Shape));
    self->material = malloc(sizeof(Material));
    *self->material = defaultMaterial();
    self->transform = malloc(sizeof(Matrix_4x4));
    *self->transform = matrix_identity_float4x4;
    self->funcTab = NULL;
    return self;
}

static inline void Shape_intersect_with_ray(Shape *self, Ray *ray)
{
    self->funcTab->intersect_with_ray((void *)self, ray);
}

static inline void Shape_set_transform(Shape *self, Matrix_4x4 *transform)
{
    self->funcTab->set_transform((void *)self, transform);
}

/* static inline Vector Shape_surface_normal_at(Shape *self, Point *worldPoint)
 */
/* { */
/* return self->funcTab->surface_normal_at((void *)self, worldPoint); */
/* } */
static inline Vector Shape_local_to_world_normal(Shape *self,
                                                 Vector *localNormal)
{
    Vector worldNormal =
        simd_mul(simd_transpose(simd_inverse(*self->transform)), *localNormal);
    worldNormal.w = 0;
    return simd_normalize(worldNormal);
}
static inline void Shape_destroy(Shape *self)
{
    free(self->material);
    free(self->transform);
    free(self);
}
#endif
