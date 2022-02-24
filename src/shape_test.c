#include <main.h>

void test_ray_transform()
{
    Sphere *s = create_Sphere(make_Point(0, 0, -5), 1);
    Ray r = init_Ray(make_Point(0, 0, -5), make_Vector(0, 0, 1));
    Matrix_4x4 scaling = scaling_matrix(2, 2, 2);
    s->shape->funcTab->set_transform(s, &scaling);
    s->shape->funcTab->intersect_with_ray(s, &r);
    r.destroy_XS(&r);
    print_float4(s->shape->local_ray.origin);
    print_float4(s->shape->local_ray.directionVec);
    s->shape->funcTab->destroy(s);
    Sphere *s2 = create_Sphere(make_Point(0, 0, 0), 1);
    Matrix_4x4 transform = translation_matrix(5, 0, 0);
    s2->funcTab->set_transform(s2, &transform);
    s2->funcTab->intersect_with_ray(s2, &r);
    r.destroy_XS(&r);
    print_float4(s2->shape->local_ray.origin);
    print_float4(s2->shape->local_ray.directionVec);
    s2->shape->funcTab->destroy(s2);
    Sphere *s3 = create_Sphere(make_Point(0, 0, 0), 1);
    transform = translation_matrix(0, 1, 0);
    s3->funcTab->set_transform(s3, &transform);
    Point test = make_Point(0, 1.70711, -0.70711);
    print_float4(s3->funcTab->surface_normal_at(s3, &test));
    s3->funcTab->destroy(s3);
    transform = simd_mul(scaling_matrix(1, 0.5, 1),
                         rotation_matrix(PI / 5, simd_make_float3(0, 0, 1)));
    s3 = create_Sphere();
    s3->funcTab->set_transform(s3, &transform);
    test = make_Point(0, sqrtf(2) / 2.0, -sqrtf(2) / 2.0);
    print_float4(s3->funcTab->surface_normal_at(s3, &test));
    s3->funcTab->destroy(s3);
}

int main(int argc, char *argv[])
{
    test_ray_transform();
    return 0;
}
