#include <geometry/plane.h>

struct Shape_funcTab plane_funcTab = {
    Plane_intersect_with_ray,
    Plane_set_transform,
    Plane_surface_normal_at,
    Plane_destroy,
};

Plane *create_Plane(void)
{
    Plane *self = malloc(sizeof(Plane));
    self->shape = create_Shape();
    self->shape->funcTab = &plane_funcTab;
    self->funcTab = &plane_funcTab;
    self->ShapeType = PLANE;
    return self;
}
void Plane_intersect_with_ray(void *PlanePtr, Ray *ray)
{
    Plane *self = (Plane *)PlanePtr;
    Shape *shape = self->shape;
    Ray local_ray = ray->transform(ray, shape->transform);
    shape->local_ray = local_ray;
    if (fabs(local_ray.directionVec.y) < EPSILON)
    {
        return;
    }
    if (!ray->xs)
        ray->xs = init_intxnCollection();
    int *count = &(ray->xs->intersects_counts);
    (*count)++;
    float t = -ray->origin.y / ray->directionVec.y;
    ray->xs->intersects =
        realloc(ray->xs->intersects, sizeof(Intersect *) * (*count));
    ray->xs->intersects[*count - 1] = mark_intersect(t, self, PLANE);
    return;
}
void Plane_set_transform(void *PlanePtr, Matrix_4x4 *transformMatPtr)
{
    Plane *self = (Plane *)PlanePtr;
    *self->shape->transform =
        simd_mul(*transformMatPtr, *self->shape->transform);
}
Vector Plane_surface_normal_at(const void *PlanePtr, Point *worldPoint)
{
    Plane *self = (Plane *)PlanePtr;
    Vector localNormal = make_Vector(0, 1, 0); // A XZ-plane
    return Shape_local_to_world_normal(self->shape, &localNormal);
}
void Plane_destroy(void *PlanePtr)
{
    Plane *self = (Plane *)PlanePtr;
    Shape_destroy(self->shape);
    free(self);
}
