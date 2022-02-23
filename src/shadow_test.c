#include <main.h>

static void test_shadow()
{
    World *w = init_default_world();
    simd_float4 p1 = {0, 10, 0, 1};
    bool shadowed = w->funcTab->is_shadowed(w, &p1);
    printf("Is p1 shadowed? %s\n", (shadowed == true) ? "True" : "False");
    simd_float4 p2 = {10, -10, 10, 1};
    shadowed = w->funcTab->is_shadowed(w, &p2);
    printf("Is p2 shadowed? %s\n", (shadowed == true) ? "True" : "False");
    simd_float4 p3 = {-20, 20, -20, 1};
    shadowed = w->funcTab->is_shadowed(w, &p3);
    printf("Is p3 shadowed? %s\n", (shadowed == true) ? "True" : "False");
    simd_float4 p4 = {-2, 2, -2, 1};
    shadowed = w->funcTab->is_shadowed(w, &p4);
    printf("Is p4 shadowed? %s\n", (shadowed == true) ? "True" : "False");
    // Test shading
    w->lights[0]->pos = (simd_float4){0, 0, -10, 1};
    simd_float4x4 transform_S2 =
        simd_mul(translation_matrix((simd_float3){0, 0, 10}),
                 scaling_matrix((simd_float3){2, 2, 2}));
    w->sphereArray[1]->transform = transform_S2;
    w->sphereArray[1]->m.color = (Color){1, 1, 1};
    Ray r = init_Ray((simd_float4){0, 0, 5, 1}, (simd_float4){0, 0, 1, 0});
    r.intersects_with_Sphere(&r, w->sphereArray[1]);
    for (int i = 0; i < r.xs->intersects_counts; ++i)
    {
        printf("%f\n", r.xs->intersects[i]->t);
        PreComp *comp = prepare_computations(r.xs->intersects[i], &r);
        Color c = w->funcTab->shade_hit(w, comp);
        print_float3(c);
        comp->destroy(comp);
    }
    r.destroy_XS(&r);
    w->funcTab->destroy(w);
    r = init_Ray((simd_float4){0, 0, -5, 1}, (simd_float4){0, 0, 1, 0});
    Sphere s = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    s.transform = translation_matrix((simd_float3){0, 0, 1});
    r.intersects_with_Sphere(&r, &s);
    PreComp *cp = prepare_computations(r.xs->intersects[0], &r);
    printf("%s\n", (cp->over_point->z < -EPSILON / 2) ? "True" : "False");
    printf("%f, %f\n", cp->point->z, cp->over_point->z);
    cp->destroy(cp);
    r.destroy_XS(&r);
}

int main(int argc, char *argv[])
{
    test_shadow();
    return 0;
}
