#pragma once
#include <geometry/geometry.h>
#include <lights/lights.h>
#include <ray/ray.h>
typedef struct world World;
struct world
{
    Light *light;
    Sphere **sphereArray; // Pointer to arrays of Sphere pointer
    int sphereCounts;
    void (*func_destroy)(World *self);
    IntersectCollection *(*func_intersect_world)(World *self, Ray *rayPtr);
};

/* @abstract: Init a default world with a lightsource at (-10, -10, -10)
 * and two spheres at the origin, one is a unit sphere and another one in the
 * inner has a radius of 0.5
 */
static inline World *init_default_world(void);
/* @abstract: Release all memory in the World instance */
static inline void destroy_world(World *self);
/* @abstrac: Returns the IntersectCollection ptr of a ray intersects with
 * the world.
 */
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr);

#pragma mark -Implementation
static inline World *init_default_world(void)
{
    World *new = malloc(sizeof(World));
    Light *light = malloc(sizeof(Light));
    *light = point_light((simd_float4){-10, -10, -10, 1}, (Color){1, 1, 1});
    new->light = light;
    new->sphereArray = malloc(2 * sizeof(Sphere *));
    for (int i = 0; i < 2; ++i)
    {
        new->sphereArray[i] = malloc(sizeof(Sphere));
        *(new->sphereArray[i]) = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    }
    Sphere *s1 = new->sphereArray[0];
    Sphere *s2 = new->sphereArray[1];
    s2->r = 0.5;
    s1->m.color = (Color){0.8, 1.0, 0.6};
    s1->m.diffuse = 0.7;
    s1->m.specular = 0.2;
    new->sphereCounts = 2;
    new->func_destroy = destroy_world;
    new->func_intersect_world = intersect_world;
    return new;
}
static inline void destroy_world(World *self)
{
    free(self->light);
    for (int i = 0; i < self->sphereCounts; ++i)
    {
        free(self->sphereArray[i]);
    }
    free(self->sphereArray);
    free(self);
}
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr)
{
    for (int i = 0; i < self->sphereCounts; ++i)
    {
        rayPtr->intersects_with_Sphere(rayPtr, self->sphereArray[i]);
    }
    sort_xs(rayPtr->xs);
    return rayPtr->xs;
}
