#pragma once
#include <geometry/shape.h>
#include <geometry/type_common.h>
#include <ray/ray.h>
struct Cube
{
    Shape shape;
};

/* @abstract: Initialize a cube, located at the origin and
 * extends from -1 to +1 in each axis. */
Cube *create_Cube(void);
/* @abstract: Calculate intersect with a cube */
void Cube_intersect_with_ray(Shape *CubeSelf, Ray *r);
/* @abstract: Apply a transform to the cube */
void Cube_set_transform(Shape *CubeSelf, Matrix_4x4 *transformMatPtr);
Vector Cube_surface_normal_at(const Shape *CubeSelf, Point *worldPoint);
void Cube_destroy(Shape *CubeSelf);
