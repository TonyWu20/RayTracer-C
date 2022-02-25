#include <main.h>

void test_stripe()
{
    Pattern *stripe = stripe_pattern();
    print_float3(stripe->color_a);
    print_float3(stripe->color_b);
    Point p1 = make_Point(0, 0, 0);
    stripe->funcTab->destroy((Pattern *)stripe);
}

int main(int argc, char *argv[])
{
    test_stripe();
    return 0;
}
