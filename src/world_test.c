#include <main.h>

static void test_world()
{
    World *w = init_default_world();
    Ray r = init_Ray((simd_float4){0, 0, -5, 1}, (simd_float4){0, 0, 1, 0});
    IntersectCollection *xs = w->func_intersect_world(w, &r);
    printf("Intersects count: %d\n", xs->intersects_counts);
    for (int i = 0; i < xs->intersects_counts; ++i)
    {
        printf("xs[%d].t = %f\n", i, xs->intersects[i]->pos.z);
    }
    r.destroy_XS(&r);
    w->func_destroy(w);
}

int main(int argc, char *argv[])
{
    test_world();
    return 0;
}
