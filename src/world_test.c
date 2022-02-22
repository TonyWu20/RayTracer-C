#include <main.h>

static void test_world()
{
    World *w = init_default_world();
    Ray r = init_Ray((simd_float4){0, 0, -5, 1}, (simd_float4){0, 1, 0, 0});
    printf("Color when a ray misses ");
    print_float3(color_at(w, &r));
    r.destroy_XS(&r);
    r.directionVec = (simd_float4){0, 0, 1, 0};
    printf("Color when a ray hits ");
    print_float3(color_at(w, &r));
    r.destroy_XS(&r);
    w->sphereArray[0]->m.ambient = 1;
    w->sphereArray[1]->m.ambient = 1;
    r = init_Ray((simd_float4){0, 0, 0.75, 1}, (simd_float4){0, 0, -1, 0});
    printf("Color with an intersection behind the ray ");
    print_float3(color_at(w, &r));
    print_float3(w->sphereArray[1]->m.color);
    r.destroy_XS(&r);
    w->funcTab->destroy(w);
}

int main(int argc, char *argv[])
{
    test_world();
    return 0;
}
