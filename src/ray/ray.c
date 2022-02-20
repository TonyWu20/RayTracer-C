#include <ray/ray.h>

int intersects_with_Sphere(Ray *ray, const Sphere *s)
{
    simd_float4 sphere_to_ray = ray->origin - s->origin;
    float a = simd_dot(ray->directionVec, ray->directionVec);
    float b = 2 * simd_dot(ray->directionVec, sphere_to_ray);
    float c = simd_dot(sphere_to_ray, sphere_to_ray) - s->r * s->r;
    float discriminant = b * b - 4 * a * c;
    float t1 = -b - sqrtf(discriminant) / (2 * a);
    float t2 = -b + sqrtf(discriminant) / (2 * a);
    if (discriminant < 0)
    {
        return -1;
    }
    if (!ray->intxnSphere)
        ray->intxnSphere = init_intxnCollection();
    int intxnCounts = ray->intxnSphere->intersects_counts; // Current Count
    ray->intxnSphere->intersects =
        realloc(ray->intxnSphere->intersects,
                sizeof(SphereIntersect *) * (intxnCounts + 2)); // Extend by 2
    ray->intxnSphere->intersects_counts += 2; // Add count by 2
    for (int i = 0; i < 2; ++i)
    {
        ray->intxnSphere->intersects[intxnCounts + i] =
            malloc(sizeof(SphereIntersect));
        ray->intxnSphere->intersects[intxnCounts + i]->t = (i == 0) ? t1 : t2;
        ray->intxnSphere->intersects[intxnCounts + i]->pos =
            (i == 0) ? currPosition(ray, t1) : currPosition(ray, t2);
        ray->intxnSphere->intersects[intxnCounts + i]->object = s;
    }
    return 1;
}

static int SphereIntersectCmp(const void *a, const void *b)
{
    SphereIntersect *aPtr = *(SphereIntersect **)a;
    SphereIntersect *bPtr = *(SphereIntersect **)b;
    if (aPtr->t - bPtr->t > 0)
        return 1;
    else if (aPtr->t == bPtr->t)
        return 0;
    else
        return -1;
}
SphereIntersect *hit_Sphere(IntersectCollection *collection)
{
    qsort(collection->intersects, collection->intersects_counts,
          sizeof(SphereIntersect *), SphereIntersectCmp);
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
