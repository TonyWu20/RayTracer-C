#include <geometry/sphere.h>

struct Shape_funcTab Sphere_funcTab = {
    &Sphere_intersect_with_ray, &Sphere_set_transform,
    &Sphere_surface_normal_at, &Sphere_destroy};

FUNC Sphere *create_Sphere(Point origin, float r)
{
    Sphere *self = malloc(sizeof(Sphere));
    self->shape = create_Shape();
    self->origin = origin;
    self->r = r;
    self->shape->funcTab = &Sphere_funcTab;
    self->funcTab = self->shape->funcTab;
    return self;
}
FUNC Sphere *create_Sphere(void)
{
    return create_Sphere(make_Point(0, 0, 0), 1);
}

void Sphere_intersect_with_ray(void *SphereSelf, Ray *r)
{
    Sphere *self = (Sphere *)SphereSelf;
    Shape *shape = self->shape;
    shape->local_ray = r->transform(r, shape->transform);
    Vector sphere_to_ray = shape->local_ray.origin - self->origin;
    float a =
        simd_dot(shape->local_ray.directionVec, shape->local_ray.directionVec);
    float b = 2 * simd_dot(shape->local_ray.directionVec, sphere_to_ray);
    float c = simd_dot(sphere_to_ray, sphere_to_ray) - self->r * self->r;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return;
    }
    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);
    if (!r->xs)
        r->xs = init_intxnCollection();
    int *count = &r->xs->intersects_counts;
    for (int i = 0; i < 2; ++i)
    {
        (*count)++;
        r->xs->intersects =
            realloc(r->xs->intersects, sizeof(Intersect *) * (*count));
        r->xs->intersects[*count - 1] =
            mark_intersect((i == 0) ? t1 : t2, self, SPHERE);
    }
}
void Sphere_set_transform(void *selfPtr, Matrix_4x4 *transformMatPtr)
{
    Sphere *self = (Sphere *)selfPtr;
    *self->shape->transform =
        simd_mul(*transformMatPtr, *self->shape->transform);
}
Vector Sphere_surface_normal_at(const void *selfPtr, Point *worldPoint)
{
    if (worldPoint->w != 1)
    {
        fprintf(stderr, "Not a valid Point (w!=1)\n");
        return (Vector){0, 0, 0, 0};
    }
    const Sphere *self = (const Sphere *)selfPtr;
    Point localPoint =
        simd_mul(simd_inverse(*self->shape->transform), *worldPoint);
    Vector localNormal = localPoint - self->origin;
    Vector worldNormal = Shape_local_to_world_normal(self->shape, &localNormal);
    return worldNormal;
}
void Sphere_destroy(void *selfPtr)
{
    Sphere *self = (Sphere *)selfPtr;
    Shape_destroy(self->shape);
    free(self); // malloced in create_Sphere()
}
