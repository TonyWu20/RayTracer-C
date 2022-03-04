#pragma once
#include <colors/colors.h>
#include <geometry/geometry.h>
#include <lights/lights.h>
#include <ray/ray.h>
#include <stdbool.h>
#include <string.h>
#include <types/types.h>
#include <world/world_helper.h>

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
    Vector *reflectV;
    Point *over_point;
    Point *under_point;
    float n1;
    float n2;
    bool inside;
    void (*destroy)(PreComp *self);
};
struct world_vtable
{
    void (*destroy)(World *self);
    IntersectCollection *(*intersect_world)(World *self, Ray *rayPtr);
    Color (*shade_hit)(World *self, PreComp *comp, int to_reflect);
    void (*add_light)(World *self, Light *light);
    void (*add_object)(World *self, Shape *shapeObj);
    Color (*color_at)(World *self, Ray *r, int to_reflect);
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
static inline Color shade_hit(World *self, PreComp *comp, int to_reflect);
/* @abstract: Add a light source to the world, update lightCounts by one */
/* @params: World *self, Light *light */
static inline void add_light(World *self, Light *light);
/* @abstract: Add a sphere to the world, update sphereCounts by one */
/* @params: World *self, Sphere *sphere */
static inline void add_shape(World *self, Shape *shapeObj);
/* @abstract: Intersect the world with the given ray, and then return
 * the color at the intersection */
static inline Color color_at(World *self, Ray *r, int to_reflect);
/* @abstract: Reflected color */
static inline Color reflected_color(World *self, PreComp *comp, int to_reflect);
/* @abstract: Refracted color */
static inline Color refracted_color(World *self, PreComp *comp, int to_refract);
/* @abstract: Determine whether the point is in shadow */
static inline bool is_shadowed(World *self, simd_float4 *point);
/* @abstract: Schlick function to approximate Fresnel's equations */
static inline float schlick(PreComp *comp);

/* @abstrat: Release memory of a PreComp struct */
/* @abstract: Init a PreComp struct by an Intersect and a ray */
/* @params: const Intersect *, const Ray *, IntersectCollection **/
/* @Warning: Free after use! */
static inline PreComp *prepare_computations(Intersect *intxs, const Ray *r,
                                            IntersectCollection *xs);
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
static inline Color color_at(World *self, Ray *r, int to_reflect)
{
    IntersectCollection *xs = self->funcTab->intersect_world(self, r);
    if (!xs)
        return (Color){0, 0, 0};
    Intersect *hit = hit_Object(xs);
    if (!hit)
    {
        return (Color){0, 0, 0};
    }
    PreComp *comp = prepare_computations(hit, r, xs);
    Color result = shade_hit(self, comp, to_reflect);
    comp->destroy(comp);
    return result;
}
static inline Color reflected_color(World *self, PreComp *comp, int to_reflect)
{
    if (to_reflect <= 0)
        return (Color){0, 0, 0};
    if (comp->object->material->reflective == 0.0)
        return (Color){0, 0, 0};
    Ray reflect_ray = init_Ray(*comp->over_point, *comp->reflectV);
    Color color = color_at(self, &reflect_ray, to_reflect - 1);
    return color * comp->object->material->reflective;
}
static inline Color refracted_color(World *self, PreComp *comp, int to_refract)
{
    if (to_refract <= 0)
        return (Color){0, 0, 0};
    if (comp->object->material->transparency == 0.0)
        return (Color){0, 0, 0};
    float n_ratio = comp->n1 / comp->n2;
    float cos_i = simd_dot(*comp->eyeV, *comp->normalV);
    float sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
    if (sin2_t > 1)
        return (Color){0, 0, 0};
    Vector direction_refract =
        simd_refract(*comp->eyeV, *comp->normalV, n_ratio);
    Ray refract_ray = init_Ray(*comp->under_point, direction_refract);
    Color color = color_at(self, &refract_ray, to_refract - 1) *
                  comp->object->material->transparency;
    return color;
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

static inline float schlick(PreComp *comp)
{
    float cos_eye_normal = simd_dot(*comp->eyeV, *comp->normalV);
    if (comp->n1 > comp->n2)
    {
        float n = comp->n1 / comp->n2;
        float sin2_t = pow(n, 2) * (1 - pow(cos_eye_normal, 2));
        if (sin2_t > 1)
            return 1.0;
        float cos_t = sqrtf(1 - sin2_t);
        cos_eye_normal = cos_t; // when n1 > n2 use cos(theta_t) instead
    }
    float r0 = pow(((comp->n1 - comp->n2) / (comp->n1 + comp->n2)), 2);
    float result = r0 + (1 - r0) * pow((1 - cos_eye_normal), 5);
    return result;
}
static int cmp_shape(const void *a, const void *b)
{
    return memcmp(a, b, sizeof(Shape *));
}
static inline PreComp *prepare_computations(Intersect *hit, const Ray *r,
                                            IntersectCollection *xs)
{
    PreComp *comp = malloc(sizeof(PreComp));
    if (!comp)
    {
        fprintf(stderr, "Not enough memory\n");
    }
    struct ShapeList containers = init_ShapeList();
    for (int i = 0; i < xs->intersects_counts; ++i)
    {
        if (xs->intersects[i] == hit)
        {
            if (containers.length == 0)
                comp->n1 = 1.0;
            else
                comp->n1 = containers.tail->object->material->refractive_index;
        }
        struct ShapeList_Node *np =
            find_obj(&containers, xs->intersects[i]->object);
        if (np)
            delete_obj_from_list(&containers, np);
        else
            add_obj(&containers, xs->intersects[i]->object);
        if (xs->intersects[i] == hit)
        {
            if (containers.length == 0)
                comp->n2 = 1.0;
            else
                comp->n2 = containers.tail->object->material->refractive_index;
            break;
        }
    }
    destroy_ShapeList(&containers);
    comp->t = hit->t;
    comp->object = hit->object;
    comp->point = malloc(sizeof(Point));
    *comp->point = currPosition(r, hit->t);
    comp->eyeV = malloc(sizeof(Vector));
    *comp->eyeV = -r->directionVec; // the directionVec has been normalized
    comp->eyeV->w = 0;
    comp->normalV = malloc(sizeof(Vector));
    *comp->normalV =
        comp->object->funcTab->surface_normal_at(comp->object, comp->point);
    // The returned surface normal is already normalized
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
    comp->reflectV = malloc(sizeof(Vector));
    *comp->reflectV = simd_reflect(r->directionVec, *comp->normalV);
    comp->under_point = malloc(sizeof(Point));
    *(comp->under_point) = *(comp->point) - *(comp->normalV) * 10 * EPSILON;
    comp->destroy = destroy_precomp;
    return comp;
}
static inline void destroy_precomp(PreComp *self)
{
    free(self->point);
    free(self->eyeV);
    free(self->normalV);
    free(self->over_point);
    free(self->under_point);
    free(self->reflectV);
    free(self);
}
static inline Color shade_hit(World *self, PreComp *comp, int remaining)
{
    Color result = {0, 0, 0};
    bool shadowed = self->funcTab->is_shadowed(self, comp->over_point);
    for (int i = 0; i < self->lightCounts; ++i)
    {
        result +=
            lighting(comp->object->material, comp->object, self->lights[i],
                     comp->point, comp->eyeV, comp->normalV, shadowed);
    }
    Color reflected = reflected_color(self, comp, remaining);
    Color refracted = refracted_color(self, comp, remaining);
    Material *m = comp->object->material;
    if (m->reflective > 0 && m->transparency > 0)
    {
        float reflectance = schlick(comp);
        Color res =
            result + reflected * reflectance + refracted * (1 - reflectance);
        return res;
    }
    return result + reflected + refracted;
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
