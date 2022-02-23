#include <main.h>

void default_orient()
{
    simd_float4 from = {0, 0, 0, 1};
    simd_float4 to = {0, 0, -1, 1};
    simd_float4 upVector = {0, 1, 0, 0};
    simd_float4x4 view = view_transform(&from, &to, &upVector);
    print_float4x4(view);
}
void look_in_positive_z()
{
    simd_float4 from = {0, 0, 0, 1};
    simd_float4 to = {0, 0, 1, 1};
    simd_float4 upVector = {0, 1, 0, 0};
    simd_float4x4 view = view_transform(&from, &to, &upVector);
    print_float4x4(view);
}
void move_the_world()
{
    simd_float4 from = {0, 0, 8, 1};
    simd_float4 to = {0, 0, 0, 1};
    simd_float4 upVector = {0, 1, 0, 0};
    simd_float4x4 view = view_transform(&from, &to, &upVector);
    print_float4x4(view);
}
void arbitray_view()
{
    simd_float4 from = {1, 3, 2, 1};
    simd_float4 to = {4, -2, 8, 1};
    simd_float4 upVector = {1, 1, 0, 0};
    simd_float4x4 view = view_transform(&from, &to, &upVector);
    print_float4x4(view);
}

int main(int argc, char *argv[])
{
    default_orient();
    look_in_positive_z();
    move_the_world();
    arbitray_view();
    return 0;
}
