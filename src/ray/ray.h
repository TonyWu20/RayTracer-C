#pragma once
#include <geometry/sphere.h>
#include <stdlib.h>
#include <types/types.h>
typedef struct IntersectCollection IntersectCollection;
typedef struct SphereIntersection SphereIntersect;
typedef struct Ray Ray;

struct Ray
{
    Point origin;
    Vector directionVec;
    IntersectCollection *xs;
    int (*intersects_with_Sphere)(Ray *self, const Sphere *s);
    Ray (*transform)(Ray *self, simd_float4x4 *transformMatrix);
    void (*destroy_XS)(Ray *self);
};

struct SphereIntersection
{
    float t;
    Point pos;
    const Sphere *object;
};

struct IntersectCollection
{
    SphereIntersect **intersects;
    int intersects_counts;
    void (*destroy)(IntersectCollection *self);
};

/* @abstract: Init a Ray object */
static inline Ray init_Ray(Point origin, Vector directionVec);
/* @abstract: Get the current position of the Ray after time */
static inline void destroy_XS(Ray *self);
static inline Point currPosition(const Ray *ray, float distance);
/* @abstract: Returns a pointer of pointers of Intersect struct
 * @params: (const Ray *) ray, (const Sphere *) s, (int *) returnSize
 * @MallocUsed: Free must be called to the (Intersect **)pptr and (Intersect
 * *)ptr
 */
static inline IntersectCollection *init_intxnCollection(void);
/* @abstract: Apply a transformation to the Ray, returns a new Ray */
static inline Ray apply_transformation(Ray *self, Matrix_4x4 *transformMatrix);
/* @abstrat: Sort the IntersectCollection in ascending order */
static inline void sort_xs(IntersectCollection *self);
/* @abstract: Release memory occupied by IntersectCollection */
static inline void destroy_intxnCollection(IntersectCollection *self);
/* @abstract: Compute intersects with the given sphere and add to
 * collection if valid
 */
int intersects_with_Sphere(Ray *self, const Sphere *s);
/* @abstract: Returns the SphereIntersect with the smallest nonnegative
 * t value
 */
SphereIntersect *hit_Sphere(IntersectCollection *intersects);

static inline Ray init_Ray(Point origin, Vector directionVec)
{
    Ray result;
    result.origin = origin;
    result.directionVec = directionVec;
    result.xs = NULL;
    result.intersects_with_Sphere = intersects_with_Sphere;
    result.transform = apply_transformation;
    result.destroy_XS = destroy_XS;
    return result;
}
static inline void destroy_XS(Ray *self)
{
    if (self->xs)
        self->xs->destroy(self->xs);
}
static inline Point currPosition(const Ray *ray, float distance)
{
    return ray->directionVec * distance + ray->origin;
}

static inline Ray apply_transformation(Ray *self, Matrix_4x4 *transformMatrix)
{
    Matrix_4x4 rayTransform = simd_inverse(*transformMatrix);
    Ray new = init_Ray(self->origin, self->directionVec);
    new.origin = simd_mul(rayTransform, new.origin);
    new.directionVec = simd_mul(rayTransform, new.directionVec);
    return new;
}

/* @abstract: Returns a pointer of pointers of Intersect struct
 * @params: (const Ray *) ray, (const Sphere *) s, (int *) returnSize
 * @MallocUsed: Free must be called to the (Intersect **)pptr and (Intersect
 * *)ptr
 */
static inline IntersectCollection *init_intxnCollection(void)
{
    IntersectCollection *result = malloc(sizeof(IntersectCollection));
    result->intersects = NULL;
    result->intersects_counts = 0;
    result->destroy = destroy_intxnCollection;
    return result;
}
static inline void destroy_intxnCollection(IntersectCollection *self)
{
    for (int i = 0; i < self->intersects_counts; ++i)
    {
        free(self->intersects[i]);
    }
    free(self->intersects);
    free(self);
}

static int SphereIntersectCmp(const void *a, const void *b)
{
    SphereIntersect *aPtr = *(SphereIntersect **)a;
    SphereIntersect *bPtr = *(SphereIntersect **)b;
    if (aPtr->t - bPtr->t > 0)
        return 1;
    else if (fabsf(aPtr->t - bPtr->t) < 0.0001)
        return 0;
    else
        return -1;
}
static inline void sort_xs(IntersectCollection *self)
{
    qsort(self->intersects, self->intersects_counts, sizeof(SphereIntersect *),
          SphereIntersectCmp);
}
