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
    self->shape.funcTab = &plane_funcTab;
    return self;
}
void Plane_intersect_with_ray(Shape *PlanePtr, Ray *ray)
{
    Plane *self = (Plane *)PlanePtr;
    Shape *shape = (Shape *)self;
    if (fabs(shape->local_ray.directionVec.y) < 10*EPSILON)
    {
        return;
    }
    if (!ray->xs)
        ray->xs = init_intxnCollection();
    int *count = &(ray->xs->intersects_counts);
    (*count)++;
    float t = -(shape->local_ray.origin.y) / shape->local_ray.directionVec.y;
    ray->xs->intersects =
        realloc(ray->xs->intersects, sizeof(Intersect *) * (*count));
    ray->xs->intersects[*count - 1] = mark_intersect(t, (Shape *)self);
    return;
}
void Plane_set_transform(Shape *PlanePtr, Matrix_4x4 *transformMatPtr)
{
    Plane *self = (Plane *)PlanePtr;
    *self->shape.transform =
        simd_mul(*transformMatPtr, *self->shape.transform);
}
Vector Plane_surface_normal_at(const Shape *PlanePtr, Point *worldPoint)
{
    Plane *self = (Plane *)PlanePtr;
    Vector localNormal = make_Vector(0, 1, 0); // A XZ-plane
    return Shape_local_to_world_normal(&self->shape, &localNormal);
}
void Plane_destroy(Shape *PlanePtr)
{
    Plane *self = (Plane *)PlanePtr;
    Shape_destroy(&self->shape);
    free(self);
}
