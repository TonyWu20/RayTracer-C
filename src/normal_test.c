#include <main.h>
#define PI (atan(1) * 4)

static void test_normal()
{
    Sphere s =
        create_Sphere((simd_float4){0, 0, 0, 1}, 1); // unit sphere at origin
    simd_float4 point1, point2, point3, point4;
    point1 = (simd_float4){1, 0, 0, 1}; // on the x axis
    point2 = (simd_float4){0, 1, 0, 1}; // on the y axis
    point3 = (simd_float4){0, 0, 1, 1}; // on the z axis
    point4 =
        (simd_float4){sqrt(1.0 / 3.0), sqrt(1.0 / 3.0), sqrt(1.0 / 3.0), 1};
    print_float4(s.surface_normal_at(&s, &point1));
    print_float4(s.surface_normal_at(&s, &point2));
    print_float4(s.surface_normal_at(&s, &point3));
    print_float4(s.surface_normal_at(&s, &point4));
}
static void test_on_transformed()
{
    Sphere s =
        create_Sphere((simd_float4){0, 0, 0, 1}, 1); // unit sphere at origin
    simd_float4x4 translation = translation_matrix((simd_float3){0, 1, 0});
    s.set_transform(&s, &translation);
    simd_float4 point1 = {0, 1.70711, -0.70711, 1};
    print_float4(s.surface_normal_at(&s, &point1));
    Sphere s2 = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 scale = scaling_matrix((simd_float3){1, 0.5, 1});
    simd_quatf rot_z = simd_quaternion(PI / 5, (simd_float3){0, 0, 1});
    simd_float4x4 rot_z_mat = simd_matrix4x4(rot_z);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            printf("%f %s", rot_z_mat.columns[j][i], (j == 3) ? "\n" : "|");
        }
    }
    simd_float4x4 m = simd_mul(scale, rot_z_mat);
    s2.set_transform(&s2, &m);
    simd_float4 point2 = {0, sqrtf(1.0 / 2.0), -1.0 * sqrtf(1.0 / 2.0), 1};
    print_float4(s2.surface_normal_at(&s2, &point2));
}

int main(int argc, char *argv[])
{
    test_normal();
    test_on_transformed();
    return 0;
}
