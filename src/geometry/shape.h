#pragma once
/* #include <geometry/sphere.h> */
// clang-format off
#include <materials/materials.h>
#include <types/common.h>
#include <geometry/patterns.h>
#include <types/types.h>
#include <ray/ray.h>
#include <stdio.h>
#include <stdarg.h>
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
    void (*intersect_with_ray)(Shape *shape, Ray *ray);
    void (*set_transform)(Shape *shape, Matrix_4x4 *transform);
    Vector (*surface_normal_at)(const Shape *self, Point *worldPoint);
    void (*destroy)(Shape *self);
};

/* @abstract: Create an abstract super class Shape */
static inline Shape create_Shape(void);
/* @abstract: Base function for intersect_with_ray */
static inline void Shape_intersect_with_ray(Shape *self, Ray *ray);
/* @abstract: Base function for set_transform */
static inline void Shape_set_transform(Shape *self, int matNum, ...);
/* @abstract: Base function for surface_normal_at */
static inline Vector Shape_local_to_world_normal(const Shape *self,
                                                 Vector *localNormal);
/* @abstract: Release memory of the Shape struct */
static inline void Shape_destroy(Shape *self);

#pragma mark -Implementation
static inline Shape create_Shape(void)
{
    Shape self;
    self.material = malloc(sizeof(Material));
    *self.material = defaultMaterial();
    self.transform = malloc(sizeof(Matrix_4x4));
    *self.transform = matrix_identity_float4x4;
    self.funcTab = NULL;
    return self;
}

static inline void Shape_intersect_with_ray(Shape *self, Ray *ray)
{
    self->local_ray = ray->transform(ray, self->transform);
    self->funcTab->intersect_with_ray(self, ray);
}

static inline void Shape_set_transform(Shape *self, int matNum, ...)
{
    va_list arguments;
    va_start(arguments, matNum);
    for (int i = 0; i < matNum; ++i)
    {
        self->funcTab->set_transform(self, va_arg(arguments, Matrix_4x4 *));
    }
    va_end(arguments);
}

static inline Vector Shape_surface_normal_at(Shape *self, Point *worldPoint)
{
    return self->funcTab->surface_normal_at(self, worldPoint);
}
static inline Vector Shape_local_to_world_normal(const Shape *self,
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
}
