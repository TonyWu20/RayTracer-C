#include "geometry/shape.h"
#include "ray/ray.h"
#include "world/world.h"
#include <main.h>

void precompute_reflect()
{
    Plane *p = create_Plane();
    Ray r = init_Ray(make_Point(0, 1, -1),
                     make_Vector(0, -sqrtf(2) / 2.0, sqrtf(2) / 2.0));
    Shape_intersect_with_ray((Shape *)p, &r);
    PreComp *comp = prepare_computations(r.xs->intersects[0], &r);
    print_float4(*comp->reflectV);
    r.destroy_XS(&r);
    comp->destroy(comp);
    p->shape.funcTab->destroy((Shape *)p);
}

void strike_nonreflective()
{
    World *w = init_default_world();
    Ray r = init_Ray(make_Point(0, 0, 0), make_Vector(0, 0, 1));
    Shape *s = w->shapeArray[1];
    s->material->ambient = 1.0;
    Shape_intersect_with_ray(s, &r);
    Intersect *hit = hit_Object(r.xs);
    PreComp *comp = prepare_computations(hit, &r);
    print_float3(reflected_color(w, comp, 0));
    comp->destroy(comp);
    r.destroy_XS(&r);
    w->funcTab->destroy(w);
}
void strike_reflective()
{
    World *w = init_default_world();
    Plane *p = create_Plane();
    Shape *pBase = (Shape *)p;
    pBase->material->reflective = 0.5;
    Matrix_4x4 translate_p = translation_matrix(0, -1, 0);
    pBase->funcTab->set_transform(pBase, &translate_p);
    w->funcTab->add_object(w, pBase);
    Ray r = init_Ray(make_Point(0, 0, -3),
                     make_Vector(0, -sqrtf(2) / 2.0, sqrtf(2) / 2.0));
    Shape_intersect_with_ray(pBase, &r);
    Intersect *hit = hit_Object(r.xs);
    PreComp *comp = prepare_computations(hit, &r);
    print_float3(reflected_color(w, comp, 0));
    comp->destroy(comp);
    r.destroy_XS(&r);
    w->funcTab->destroy(w);
}

int main(int argc, char *argv[])
{
    precompute_reflect();
    strike_nonreflective();
    strike_reflective();
    return 0;
}
