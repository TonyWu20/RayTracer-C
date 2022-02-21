#include <main.h>

static void luminanceTest(Color result)
{
    simd_float3 luminanceFactor = {0.2126, 0.7152, 0.0722};
    result = simd_clamp(result, 0, 1);
    float lum = simd_dot(luminanceFactor, result);
    printf("Luminance = %f\n", lum);
    int lumN = 11 * lum;
    printf("Output char: %c\n", ".,-~:;=!*#$@"[lumN > 0 ? lumN : 0]);
}

void test_1(
    Material m,
    simd_float4 pos) // Lighting with the eye between the light and the surface
{
    simd_float4 eyeV = {0, 0, -1, 0};
    Light light = point_light((simd_float4){0, 0, -10, 1}, (Color){1, 1, 1});
    simd_float4 normalV = {0, 0, -1, 0};
    Color result = lighting(&m, &light, &pos, &eyeV, &normalV);
    print_float3(result);
    luminanceTest(result);
}
/* Lighting with the eye between light and surface, eye offset 45 degree */
void test_2(Material m, simd_float4 pos)
{
    simd_float4 eyeV = {0, sqrtf(1.0 / 2.0), -sqrtf(1.0 / 2.0), 0};
    Light light = point_light((simd_float4){0, 0, -10, 1}, (Color){1, 1, 1});
    simd_float4 normalV = {0, 0, -1, 0};
    Color result = lighting(&m, &light, &pos, &eyeV, &normalV);
    print_float3(result);
    luminanceTest(result);
}
/* Lighting with eye opposite surface, light offset 45 degree */
void test_3(Material m, simd_float4 pos)
{
    simd_float4 eyeV = {0, 0, -1, 0};
    Light light = point_light((simd_float4){0, 10, -10, 1}, (Color){1, 1, 1});
    simd_float4 normalV = {0, 0, -1, 0};
    Color result = lighting(&m, &light, &pos, &eyeV, &normalV);
    print_float3(result);
    luminanceTest(result);
}
/* Lighting with eye in the path of the reflection vector */
void test_4(Material m, simd_float4 pos)
{
    simd_float4 eyeV = {0, -sqrtf(2.0) / 2.0, -sqrtf(2.0) / 2.0, 0};
    Light light = point_light((simd_float4){0, 10, -10, 1}, (Color){1, 1, 1});
    simd_float4 normalV = {0, 0, -1, 0};
    Color result = lighting(&m, &light, &pos, &eyeV, &normalV);
    print_float3(result);
    luminanceTest(result);
}
/* Lighting with the light behind the surface */
void test_5(Material m, simd_float4 pos)
{
    simd_float4 eyeV = {0, 0, -1};
    Light light = point_light((simd_float4){0, 0, 10, 1}, (Color){1, 1, 1});
    simd_float4 normalV = {0, 0, -1, 0};
    Color result = lighting(&m, &light, &pos, &eyeV, &normalV);
    print_float3(result);
    luminanceTest(result);
}

int main(int argc, char *argv[])
{
    Material m = defaultMaterial();
    simd_float4 pos = {0, 0, 0, 1};
    test_1(m, pos);
    test_2(m, pos);
    test_3(m, pos);
    test_4(m, pos);
    test_5(m, pos);
    return 0;
}
