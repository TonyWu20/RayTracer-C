#pragma once
#include <geometry/sphere.h>
#include <simd/simd.h>
#include <stdlib.h>
typedef struct IntersectCollection IntersectCollection;
typedef struct SphereIntersection SphereIntersect;
typedef struct Ray Ray;

struct Ray
{
    simd_float4 origin;
    simd_float4 directionVec;
    IntersectCollection *intxnSphere;
    int (*intersects_with_Sphere)(Ray *self, const Sphere *s);
    Ray (*transform)(Ray *self, simd_float4x4 *transformMatrix);
};

struct SphereIntersection
{
    float t;
    simd_float4 pos;
    const Sphere *object;
};

struct IntersectCollection
{
    SphereIntersect **intersects;
    int intersects_counts;
    void (*destroy)(IntersectCollection *self);
};

/* @abstract: Init a Ray object */
static inline Ray init_Ray(simd_float4 origin, simd_float4 directionVec);
/* @abstract: Get the current position of the Ray after time */
static inline simd_float4 currPosition(const Ray *ray, float distance);
/* @abstract: Returns a pointer of pointers of Intersect struct
 * @params: (const Ray *) ray, (const Sphere *) s, (int *) returnSize
 * @MallocUsed: Free must be called to the (Intersect **)pptr and (Intersect
 * *)ptr
 */
static inline IntersectCollection *init_intxnCollection(void);
/* @abstract: Apply a transformation to the Ray, returns a new Ray */
static inline Ray apply_transformation(Ray *self,
                                       simd_float4x4 *transformMatrix);
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

static inline Ray init_Ray(simd_float4 origin, simd_float4 directionVec)
{
    Ray result;
    result.origin = origin;
    result.directionVec = directionVec;
    result.intxnSphere = NULL;
    result.intersects_with_Sphere = intersects_with_Sphere;
    result.transform = apply_transformation;
    return result;
}
static inline simd_float4 currPosition(const Ray *ray, float distance)
{
    return ray->directionVec * distance + ray->origin;
}

static inline Ray apply_transformation(Ray *self,
                                       simd_float4x4 *transformMatrix)
{
    simd_float4x4 rayTransform = simd_inverse(*transformMatrix);
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
    IntersectCollection *result = malloc(sizeof(*result));
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
    free(self);
}
