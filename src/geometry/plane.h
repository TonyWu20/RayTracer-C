#pragma once
#include <geometry/shape.h>
#include <geometry/type_common.h>
#include <ray/ray.h>
struct Plane
{
    Shape shape;
    Ray local_ray;
};

/* @abstract: Initialize a plane, extending in XZ plane infinitely */
Plane *create_Plane(void);
/* @abstract: Calculate intersect with a plane */
/* update IntersectCollection *xs in *r */
void Plane_intersect_with_ray(Shape *PlaneSelf, Ray *r);
/* @abstract: Apply a transform to the plane */
void Plane_set_transform(Shape *PlanePtr, Matrix_4x4 *transformMatPtr);
Vector Plane_surface_normal_at(const Shape *PlaneSelf, Point *worldPoint);
void Plane_destroy(Shape *selfPtr);
