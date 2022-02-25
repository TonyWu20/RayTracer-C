#pragma once
#include <colors/colors.h>
#include <geometry/geometry.h>
#include <lights/lights.h>
#include <ray/ray.h>
#include <stdbool.h>
#include <types/types.h>

/* typedef section */
typedef struct world World;
typedef struct PreComp PreComp;
/* typedef section ends */

struct world
{
    Light **lights;
    int lightCounts;
    Shape **shapeArray; // Pointer to arrays of Sphere pointer
    int shapeCounts;
    struct world_vtable *funcTab;
};

struct PreComp
{
    float t;
    const Shape *object;
    Point *point;
    Vector *eyeV;
    Vector *normalV;
    Point *over_point;
    bool inside;
    void (*destroy)(PreComp *self);
};
struct world_vtable
{
    void (*destroy)(World *self);
    IntersectCollection *(*intersect_world)(World *self, Ray *rayPtr);
    Color (*shade_hit)(World *self, PreComp *comp);
    void (*add_light)(World *self, Light *light);
    void (*add_object)(World *self, Shape *shapeObj);
    Color (*color_at)(World *self, Ray *r);
    bool (*is_shadowed)(World *self, simd_float4 *point);
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
static inline void add_shape(World *self, Shape *shapeObj);
/* @abstract: Intersect the world with the given ray, and then return
 * the color at the intersection */
static inline Color color_at(World *self, Ray *r);
/* @abstract: Determine whether the point is in shadow */
static inline bool is_shadowed(World *self, simd_float4 *point);

/* @abstrat: Release memory of a PreComp struct */
/* @abstract: Init a PreComp struct by an Intersect and a ray */
/* @params: const Intersect *, const Ray * */
/* @Warning: Free after use! */
static inline PreComp *prepare_computations(Intersect *intxs, const Ray *r);
static inline void destroy_precomp(PreComp *self);
/* @abstrac: Returns the IntersectCollection ptr of a ray intersects with
 * the world.
 * @Warning: The return IntersectCollection does not need to free immediately.
 * It will be freed when the Ray * is destroyed. */
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr);

#pragma mark -Implementation
struct world_vtable WorldVtable = {
    destroy_world, intersect_world, shade_hit,   add_light,
    add_shape,     color_at,        is_shadowed,
};
static inline World *init_default_world(void)
{
    World *new = malloc(sizeof(World));
    new->lights = malloc(sizeof(Light *));
    Light *light = malloc(sizeof(Light));
    /* *light = point_light((simd_float4){-10, 10, -10, 1}, (Color){1, 1, 1});
     */
    *light = point_light(make_Point(-10, 10, -10), (Color){1, 1, 1});
    new->lights[0] = light;
    new->lightCounts = 1;
    new->shapeArray = malloc(2 * sizeof(Sphere *));
    for (int i = 0; i < 2; ++i)
    {
        (new->shapeArray[i]) = (Shape *)create_Sphere((Point){0, 0, 0, 1}, 1);
    }
    Shape *s1 = new->shapeArray[0];
    Shape *s2 = new->shapeArray[1];
    simd_float4x4 scale_2 = scaling_matrix(0.5, 0.5, 0.5);
    s2->funcTab->set_transform(s2, &scale_2);
    s1->material->color = (Color){0.8, 1.0, 0.6};
    s1->material->diffuse = 0.7;
    s1->material->specular = 0.2;
    s2->material->color = (Color){1, 0, 0};
    new->shapeCounts = 2;
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
    for (int i = 0; i < self->shapeCounts; ++i)
    {
        self->shapeArray[i]->funcTab->destroy(self->shapeArray[i]);
    }
    free(self->shapeArray);
    free(self);
}
static inline void add_light(World *self, Light *light)
{
    self->lightCounts++;
    self->lights = realloc(self->lights, self->lightCounts * sizeof(Light *));
    self->lights[self->lightCounts - 1] = malloc(sizeof(Light));
    *(self->lights[self->lightCounts - 1]) = *light;
}
static inline void add_shape(World *self, Shape *shapeObj)
{
    self->shapeCounts++;
    self->shapeArray =
        realloc(self->shapeArray, self->shapeCounts * sizeof(Shape *));
    self->shapeArray[self->shapeCounts - 1] = shapeObj;
}
static inline Color color_at(World *self, Ray *r)
{
    IntersectCollection *xs = self->funcTab->intersect_world(self, r);
    if (!xs)
        return (Color){0, 0, 0};
    Intersect *hit = hit_Object(xs);
    if (!hit)
    {
        return (Color){0, 0, 0};
    }
    PreComp *comp = prepare_computations(hit, r);
    Color result = shade_hit(self, comp);
    comp->destroy(comp);
    return result;
}
static inline bool is_shadowed(World *self, simd_float4 *point)
{
    for (int i = 0; i < self->lightCounts; ++i) // Support multi light sources
    {
        Vector vec = self->lights[i]->pos - *point;
        float distance = simd_length(vec);
        Vector direction = simd_normalize(vec);
        Ray r_to_light = init_Ray(*point, direction);
        IntersectCollection *xs =
            self->funcTab->intersect_world(self, &r_to_light);
        if (!xs)
        {
            r_to_light.destroy_XS(&r_to_light);
            return false;
        }
        else
        {
            Intersect *hit = hit_Object(xs);
            if (hit && hit->t > distance)
            {
                r_to_light.destroy_XS(&r_to_light);
                return false;
            }
            else if (!hit)
            {
                r_to_light.destroy_XS(&r_to_light);
                return false;
            }
        }
        r_to_light.destroy_XS(&r_to_light);
    }
    return true;
}
static inline PreComp *prepare_computations(Intersect *intxs, const Ray *r)
{
    PreComp *comp = malloc(sizeof(PreComp));
    if (!comp)
    {
        fprintf(stderr, "Not enough memory\n");
    }
    comp->t = intxs->t;
    comp->object = intxs->object;
    comp->point = malloc(sizeof(Point));
    *comp->point = currPosition(r, intxs->t);
    comp->eyeV = malloc(sizeof(Vector));
    *comp->eyeV = simd_make_float4(simd_make_float3(-r->directionVec), 0);
    comp->normalV = malloc(sizeof(Vector));
    *comp->normalV =
        comp->object->funcTab->surface_normal_at(comp->object, comp->point);
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
    comp->over_point = malloc(sizeof(Point));
    *comp->over_point = *comp->point + *comp->normalV * 10 * EPSILON;
    comp->destroy = destroy_precomp;
    return comp;
}
static inline void destroy_precomp(PreComp *self)
{
    free(self->point);
    free(self->eyeV);
    free(self->normalV);
    free(self->over_point);
    free(self);
}
static inline Color shade_hit(World *self, PreComp *comp)
{
    Color result = {0, 0, 0};
    bool shadowed = self->funcTab->is_shadowed(self, comp->over_point);
    for (int i = 0; i < self->lightCounts; ++i)
    {
        result += lighting(comp->object->material, self->lights[i], comp->point,
                           comp->eyeV, comp->normalV, shadowed);
    }
    return result;
}
static inline IntersectCollection *intersect_world(World *self, Ray *rayPtr)
{
    for (int i = 0; i < self->shapeCounts; ++i)
    {
        Shape_intersect_with_ray(self->shapeArray[i], rayPtr);
    }
    if (rayPtr->xs)
    {
        sort_xs(rayPtr->xs);
        return rayPtr->xs;
    }
    else
        return NULL;
}
