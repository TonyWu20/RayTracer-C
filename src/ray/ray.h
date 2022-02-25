#pragma once
#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED
#include <geometry/type_common.h>
#include <stdlib.h>
#include <types/common.h>
#include <types/types.h>
typedef struct IntersectCollection IntersectCollection;
typedef struct Intersection Intersect;

struct Ray
{
    Point origin;
    Vector directionVec;
    IntersectCollection *xs;
    Ray (*transform)(Ray *self, simd_float4x4 *transformMatrix);
    void (*destroy_XS)(Ray *self);
};

struct Intersection
{
    float t;
    const Shape *object;
};

struct IntersectCollection
{
    Intersect **intersects;
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
/* @abstract: Create an Intersect * by t, object and ShapeType */
static inline Intersect *mark_intersect(float t, const Shape *object);
/* @abstract: Apply a transformation to the Ray, returns a new Ray */
static inline Ray apply_transformation(Ray *self, Matrix_4x4 *transformMatrix);
/* @abstrat: Sort the IntersectCollection in ascending order */
static inline void sort_xs(IntersectCollection *self);
/* @abstract: Release memory occupied by IntersectCollection */
static inline void destroy_intxnCollection(IntersectCollection *self);
/* @abstract: Returns the Intersect with the smallest nonnegative
 * t value
 */
static inline Intersect *hit_Object(IntersectCollection *collection);

static inline Ray init_Ray(Point origin, Vector directionVec)
{
    Ray result;
    result.origin = origin;
    result.directionVec = directionVec;
    result.xs = NULL;
    result.transform = apply_transformation;
    result.destroy_XS = destroy_XS;
    return result;
}
static inline void destroy_XS(Ray *self)
{
    if (self->xs)
    {
        self->xs->destroy(self->xs);
        self->xs = NULL;
    }
}
static inline Point currPosition(const Ray *ray, float distance)
{
    Point curr = ray->directionVec * distance + ray->origin;
    curr.w = 1;
    return curr;
}
static inline Intersect *mark_intersect(float t, const Shape *object)
{
    Intersect *result = malloc(sizeof(Intersect));
    result->t = t;
    result->object = object;
    return result;
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

static int IntersectCmp(const void *a, const void *b)
{
    Intersect *aPtr = *(Intersect **)a;
    Intersect *bPtr = *(Intersect **)b;
    if (aPtr->t - bPtr->t > 0)
        return 1;
    else if (fabsf(aPtr->t - bPtr->t) < 0.0001)
        return 0;
    else
        return -1;
}
static inline void sort_xs(IntersectCollection *self)
{
    qsort(self->intersects, self->intersects_counts, sizeof(Intersect *),
          IntersectCmp);
}
static inline Intersect *hit_Object(IntersectCollection *collection)
{
    sort_xs(collection);
    Intersect *result;
    for (int i = 0; i < collection->intersects_counts; ++i)
    {
        if (collection->intersects[i]->t >= 0)
        {
            result = collection->intersects[i];
            return result;
        }
    }
    return NULL;
}
#endif
