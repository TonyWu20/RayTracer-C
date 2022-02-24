#include <ray/ray.h>

int intersects_with_Sphere(Ray *ray, const Sphere *s)
{
    Ray temp_ray = ray->transform(ray, (simd_float4x4 *)(&s->transform));
    Vector sphere_to_ray = temp_ray.origin - s->origin;
    float a = simd_dot(temp_ray.directionVec, temp_ray.directionVec);
    float b = 2 * simd_dot(temp_ray.directionVec, sphere_to_ray);
    float c = simd_dot(sphere_to_ray, sphere_to_ray) - s->r * s->r;
    float discriminant = b * b - 4 * a * c;
    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    float t2 = (-b + sqrtf(discriminant)) / (2 * a);
    if (discriminant < 0)
    {
        return -1;
    }
    if (!ray->xs)
        ray->xs = init_intxnCollection();
    int *count = &ray->xs->intersects_counts;
    for (int i = 0; i < 2; ++i)
    {
        (*count)++;
        ray->xs->intersects =
            realloc(ray->xs->intersects, sizeof(SphereIntersect *) * (*count));
        ray->xs->intersects[*count - 1] = malloc(sizeof(SphereIntersect));
        if (!ray->xs->intersects[*count - 1])
        {
            fprintf(stderr, "Not enough memory\n");
        }
        ray->xs->intersects[*count - 1]->t = (i == 0) ? t1 : t2;
        ray->xs->intersects[*count - 1]->pos =
            (i == 0) ? currPosition(ray, t1) : currPosition(ray, t2);
        ray->xs->intersects[*count - 1]->object = s;
    }
    return 1;
}

SphereIntersect *hit_Sphere(IntersectCollection *collection)
{
    sort_xs(collection);
    SphereIntersect *result;
    for (int i = 0; i < collection->intersects_counts; ++i)
    {
        if (collection->intersects[i]->t > 0)
        {
            result = collection->intersects[i];
            return result;
        }
    }
    return NULL;
}
