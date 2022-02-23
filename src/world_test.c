#include <main.h>

static void test_camera_ray()
{
    World *w = init_default_world();
    Camera *cam = init_Camera(201, 101, PI / 2);
    Ray r = ray_for_pixel(cam, 100, 50);
    printf("Origin: ");
    print_float4(r.origin);
    printf("Direction: ");
    print_float4(r.directionVec);
    r = ray_for_pixel(cam, 0, 0);
    printf("Origin: ");
    print_float4(r.origin);
    printf("Direction: ");
    print_float4(r.directionVec);
    *cam->transform = simd_mul(
        simd_matrix4x4(simd_quaternion(PI / 4, (simd_float3){0, 1, 0})),
        translation_matrix((simd_float3){0, -2, 5}));
    r = ray_for_pixel(cam, 100, 50);
    printf("Origin: ");
    print_float4(r.origin);
    printf("Direction: ");
    print_float4(r.directionVec);
    cam->funcTab->destroy(cam);
    w->funcTab->destroy(w);
}

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
    printf("Color with an intersection behind the ray\n");
    print_float3(color_at(w, &r));
    print_float3(w->sphereArray[1]->m.color);
    r.destroy_XS(&r);
    w->funcTab->destroy(w);
}

void test_render()
{
    Sphere floor = create_Sphere((simd_float4){0, 0, 0}, 1.0);
    simd_float4x4 flatten_to_wall =
        scaling_matrix((simd_float3){100, 0.001, 100});
    floor.set_transform(&floor, &flatten_to_wall);
    floor.m.color = (Color){1, 0.9, 0.9};
    floor.m.specular = 0;

    Sphere left_wall = create_Sphere((simd_float4){0, 0, 0}, 1.0);
    simd_float4x4 rotation_x =
        simd_matrix4x4(simd_quaternion(PI / 2, (simd_float3){1, 0, 0}));
    simd_float4x4 rotation_y =
        simd_matrix4x4(simd_quaternion(-PI / 4, (simd_float3){0, 1, 0}));
    simd_float4x4 translateMat = translation_matrix((simd_float3){0, 0, 5});
    simd_float4x4 leftFinal =
        simd_mul(translateMat,
                 simd_mul(rotation_y,
                          simd_mul(rotation_x, simd_mul(flatten_to_wall,
                                                        left_wall.transform))));
    left_wall.set_transform(&left_wall, &leftFinal);
    /* left_wall.set_transform(&left_wall, &flatten_to_wall); */
    /* left_wall.set_transform(&left_wall, &rotation_y); */
    /* left_wall.set_transform(&left_wall, &rotation_x); */
    /* left_wall.set_transform(&left_wall, &translateMat); */
    left_wall.m.color = (Color){1, 0, 0.9};
    left_wall.m.specular = 0;

    World *w = init_default_world(); // malloc mem
    w->funcTab->add_sphere(w, &floor);
    w->funcTab->add_sphere(w, &left_wall);
    simd_float4x4 translateMiddle =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    Sphere *middle = w->sphereArray[0];
    /* middle->set_transform(w->sphereArray[0], &translateMiddle); */
    middle->m.color = (Color){0.1, 1, 0.5};
    middle->m.diffuse = 0.7;
    middle->m.specular = 0.3;
    Camera *cam = init_Camera(400, 320, PI / 1.5); // malloc mem
    simd_float4 from, to, up;
    from = simd_make_float4(-5, 1, 0, 1);
    to = simd_make_float4(0, 1, 0, 1);
    up = simd_make_float4(0, -1, 0, 0);
    simd_float4x4 transMat = view_transform(&from, &to, &up);
    *cam->transform = transMat;
    PPMCanvas *fig = cam->funcTab->render(cam, w); // malloc mem
    fig->printCanvas(fig, "world_test.ppm");
    print_float3(fig->pixel_at(fig, 200, 160));
    cam->funcTab->destroy(cam); // free malloced mem
    w->funcTab->destroy(w);     // free malloced mem
    fig->destroy(fig);          // free malloced mem
    system("convert world_test.ppm $PNG_PARAM world_test.png");
}

int main(int argc, char *argv[])
{
    test_camera_ray();
    test_world();
    test_render();
    return 0;
}
