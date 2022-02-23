#pragma once
#include <colors/colors.h>
#include <geometry/geometry.h>
#include <lights/lights.h>
#include <ray/ray.h>
#include <stdbool.h>

/* typedef section */
typedef struct world World;
typedef struct PreComp PreComp;
/* typedef section ends */

struct world
{
    Light **lights;
    int lightCounts;
    Sphere **sphereArray; // Pointer to arrays of Sphere pointer
    int sphereCounts;
    struct world_vtable *funcTab;
};

struct PreComp
{
    float t;
    const Sphere *object;
    simd_float4 *point;
    simd_float4 *eyeV;
    simd_float4 *normalV;
    bool inside;
    void (*destroy)(PreComp *self);
};
struct world_vtable
{
    void (*destroy)(World *self);
    IntersectCollection *(*intersect_world)(World *self, Ray *rayPtr);
    Color (*shade_hit)(World *self, PreComp *comp);
    void (*add_light)(World *self, Light *light);
    void (*add_sphere)(World *self, Sphere *sphere);
    Color (*color_at)(World *self, Ray *r);
};
/* @abstract: Init a default world with a lightsource at (-10, -10, -10)
 * and two spheres at the origin, one is a unit sphere and another one in the
 * inner has a radius of 0.5
 */
static inline World *init_default_world(void);
/* @abstract: Release all memory in the World instance */
static inline void destroy_world(World *self);
/* @abstract: Shading an intersection */
/* @params: World *self, PreComp *comp */
static inline Color shade_hit(World *self, PreComp *comp);
/* @abstract: Add a light source to the world, update lightCounts by one */
/* @params: World *self, Light *light */
static inline void add_light(World *self, Light *light);
/* @abstract: Add a sphere to the world, update sphereCounts by one */
/* @params: World *self, Sphere *sphere */
static inline void add_sphere(World *self, Sphere *sphere);
/* @abstract: Intersect the world with the given ray, and then return
 * the color at the intersection */
static inline Color color_at(World *self, Ray *r);
/* @abstrat: Release memory of a PreComp struct */
/* @abstract: Init a PreComp struct by an Intersect and a ray */
/* @params: const SphereIntersect *, const Ray * */
/* @Warning: Free after use! */
static inline PreComp *prepare_computations(SphereIntersect *intxs,
                                            const Ray *r);
static inline void destroy_precomp(PreComp *self);
/* @abstrac: Returns the IntersectCollection ptr of a ray intersects with
 * the world.
 * @Warning: The return IntersectCollection does not need to free immediately.
 * It will be freed when the Ray * is destroyed. */
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr);

#pragma mark -Implementation
struct world_vtable WorldVtable = {
    destroy_world, intersect_world, shade_hit, add_light, add_sphere, color_at,
};
static inline World *init_default_world(void)
{
    World *new = malloc(sizeof(World));
    new->lights = malloc(sizeof(Light *));
    Light *light = malloc(sizeof(Light));
    *light = point_light((simd_float4){-10, 10, -10, 1}, (Color){1, 1, 1});
    new->lights[0] = light;
    new->lightCounts = 1;
    new->sphereArray = malloc(2 * sizeof(Sphere *));
    for (int i = 0; i < 2; ++i)
    {
        new->sphereArray[i] = malloc(sizeof(Sphere));
        *(new->sphereArray[i]) = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    }
    Sphere *s1 = new->sphereArray[0];
    Sphere *s2 = new->sphereArray[1];
    simd_float4x4 scale_2 = scaling_matrix((simd_float3){0.5, 0.5, 0.5});
    s2->set_transform(s2, &scale_2);
    s1->m.color = (Color){0.8, 1.0, 0.6};
    s1->m.diffuse = 0.7;
    s1->m.specular = 0.2;
    s2->m.color = (Color){1, 0, 0};
    new->sphereCounts = 2;
    new->funcTab = &WorldVtable;
    return new;
}
static inline void destroy_world(World *self)
{
    for (int i = 0; i < self->lightCounts; ++i)
    {
        free(self->lights[i]);
    }
    free(self->lights);
    for (int i = 0; i < self->sphereCounts; ++i)
    {
        free(self->sphereArray[i]);
    }
    free(self->sphereArray);
    free(self);
}
static inline void add_light(World *self, Light *light)
{
    self->lightCounts++;
    self->lights = realloc(self->lights, self->lightCounts * sizeof(Light *));
    self->lights[self->lightCounts - 1] = malloc(sizeof(Light));
    *(self->lights[self->lightCounts - 1]) = *light;
}
static inline void add_sphere(World *self, Sphere *sphere)
{
    self->sphereCounts++;
    self->sphereArray =
        realloc(self->sphereArray, self->sphereCounts * sizeof(Sphere *));
    self->sphereArray[self->sphereCounts - 1] = malloc(sizeof(Sphere));
    *(self->sphereArray[self->sphereCounts - 1]) = *sphere;
}
static inline Color color_at(World *self, Ray *r)
{
    IntersectCollection *xs = self->funcTab->intersect_world(self, r);
    if (!xs)
        return (Color){0, 0, 0};
    SphereIntersect *hit = hit_Sphere(xs);
    PreComp *comp = prepare_computations(hit, r);
    Color result = shade_hit(self, comp);
    comp->destroy(comp);
    return result;
}
static inline PreComp *prepare_computations(SphereIntersect *intxs,
                                            const Ray *r)
{
    PreComp *comp = malloc(sizeof(PreComp));
    if (!comp)
    {
        fprintf(stderr, "Not enough memory\n");
    }
    comp->t = intxs->t;
    comp->object = intxs->object;
    comp->point = malloc(sizeof(simd_float4));
    *comp->point = currPosition(r, intxs->t);
    comp->eyeV = malloc(sizeof(simd_float4));
    *comp->eyeV = simd_make_float4(simd_make_float3(-r->directionVec), 0);
    comp->normalV = malloc(sizeof(simd_float4));
    *comp->normalV =
        surface_normal_at(intxs->object, (simd_float4 *)comp->point);
    if (simd_dot(*comp->normalV, *comp->eyeV) < 0)
    {
        comp->inside = true;
        *comp->normalV = -1 * *(comp->normalV);
        *comp->normalV = simd_make_float4(simd_make_float3(*comp->normalV), 0);
    }
    else
    {
        comp->inside = false;
    }
    comp->destroy = destroy_precomp;
    return comp;
}
static inline void destroy_precomp(PreComp *self)
{
    free(self->point);
    free(self->eyeV);
    free(self->normalV);
    free(self);
}
static inline Color shade_hit(World *self, PreComp *comp)
{
    Color result = {0, 0, 0};
    for (int i = 0; i < self->lightCounts; ++i)
    {
        result += lighting(&comp->object->m, self->lights[i], comp->point,
                           comp->eyeV, comp->normalV);
    }
    return result;
}
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr)
{
    for (int i = 0; i < self->sphereCounts; ++i)
    {
        rayPtr->intersects_with_Sphere(rayPtr, self->sphereArray[i]);
    }
    if (rayPtr->xs)
    {
        sort_xs(rayPtr->xs);
        return rayPtr->xs;
    }
    else
        return NULL;
}
