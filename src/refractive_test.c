#include <main.h>

void test_n1_n2()
{
    Sphere *a = glass_Sphere();
    Matrix_4x4 transform_a = scaling_matrix(2, 2, 2);
    a->shape.material->refractive_index = 1.5;
    a->shape.funcTab->set_transform(&a->shape, &transform_a);
    Sphere *b = glass_Sphere();
    Matrix_4x4 transform_b = translation_matrix(0, 0, -0.25);
    b->shape.material->refractive_index = 2.0;
    Shape *shape_b = (Shape *)b;
    shape_b->funcTab->set_transform(shape_b, &transform_b);
    Sphere *c = glass_Sphere();
    Shape *shape_c = (Shape *)c;
    shape_c->material->refractive_index = 2.5;
    Matrix_4x4 transform_c = translation_matrix(0, 0, 0.25);
    shape_c->funcTab->set_transform(shape_c, &transform_c);
    Ray r = init_Ray(make_Point(0, 0, -4), make_Vector(0, 0, 1));
    Shape_intersect_with_ray((Shape *)a, &r);
    Shape_intersect_with_ray((Shape *)b, &r);
    Shape_intersect_with_ray((Shape *)c, &r);
    sort_xs(r.xs);
    printf(" Index  n1  n2\n");
    for (int i = 0; i < r.xs->intersects_counts; ++i)
    {
        PreComp *comp = prepare_computations(r.xs->intersects[i], &r, r.xs);
        printf(" %5d %2f %2f\n", i, comp->n1, comp->n2);
        comp->destroy(comp);
    }
    r.destroy_XS(&r);
    a->shape.funcTab->destroy((Shape *)a);
    shape_b->funcTab->destroy(shape_b);
    shape_c->funcTab->destroy(shape_c);
}

void find_refracted_color()
{
    World *w = init_default_world();
    Shape *a = w->shapeArray[0];
    a->material->ambient = 1.0;
    a->material->pattern = stripe_pattern();
    Shape *b = w->shapeArray[1];
    b->material->transparency = 1.0;
    b->material->refractive_index = 1.5;
    Ray r = init_Ray(make_Point(0, 0, 0.1), make_Vector(0, 1, 0));
    for (int i = 0; i < w->shapeCounts; ++i)
    {
        Shape_intersect_with_ray(w->shapeArray[i], &r);
    }
    PreComp *comp = prepare_computations(r.xs->intersects[2], &r, r.xs);
    print_float3(refracted_color(w, comp, 5));
    r.destroy_XS(&r);
    comp->destroy(comp);
    w->funcTab->destroy(w);
}

int main(int argc, char *argv[])
{
    test_n1_n2();
    find_refracted_color();
    return 0;
}
