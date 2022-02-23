#include <main.h>

static void render_test(void)
{
    // The floor is an extremely flattened sphere with a matte texture
    Sphere floor = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 flatten_to_wall = scaling_matrix((simd_float3){10, 0.01, 10});
    floor.set_transform(&floor, &flatten_to_wall);
    floor.m.color = (Color){1, 0.9, 0.9};
    floor.m.specular = 0.1;
    // Wall on the left
    // Same scale and color of the wall, but rotated and translated
    Sphere left_wall = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    left_wall.set_transform(&left_wall, &flatten_to_wall);
    simd_float4x4 rotation_x =
        simd_matrix4x4(simd_quaternion(PI / 2, (simd_float3){1, 0, 0}));
    simd_float4x4 rotation_y =
        simd_matrix4x4(simd_quaternion(-PI / 4, (simd_float3){0, 1, 0}));
    simd_float4x4 translateMat = translation_matrix((simd_float3){0, 0, 5});
    left_wall.set_transform(&left_wall, &rotation_x);
    left_wall.set_transform(&left_wall, &rotation_y);
    left_wall.set_transform(&left_wall, &translateMat);
    left_wall.m.color = (Color){1, 0.9, 0.9};
    left_wall.m.specular = 0.1;
    // Wall on the right, rotated in opposite direction in y
    Sphere right_wall = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    right_wall.set_transform(&right_wall, &flatten_to_wall);
    right_wall.set_transform(&right_wall, &rotation_x);
    rotation_y =
        simd_matrix4x4(simd_quaternion(PI / 4, (simd_float3){0, 1, 0}));
    right_wall.set_transform(&right_wall, &rotation_y);
    right_wall.set_transform(&right_wall, &translateMat);
    right_wall.m.color = (Color){1, 0.9, 0.9};
    right_wall.m.specular = 0;
    // Create world
    World *world = init_default_world();
    // Add floor and walls
    world->funcTab->add_sphere(world, &floor);
    world->funcTab->add_sphere(world, &left_wall);
    world->funcTab->add_sphere(world, &right_wall);
    // Modify the large sphere in the middle
    simd_float4x4 translateMiddle =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    Sphere *middle = world->sphereArray[0];
    middle->set_transform(middle, &translateMiddle);
    middle->m.color = (Color){0.1, 1, 0.5}; // Green
    middle->m.diffuse = 0.7;
    middle->m.specular = 0.3;
    // Modify the smaller sphere
    simd_float4x4 translateRight =
        translation_matrix((simd_float3){1.5, 0.5, -0.5});
    Sphere *right = world->sphereArray[1];
    right->set_transform(right, &translateRight);
    right->m.color = (Color){0.5, 1, 0.1};
    right->m.diffuse = 0.7;
    right->m.specular = 0.3;
    // Smallest sphere on the left
    Sphere left = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 scale_left = scaling_matrix((simd_float3){0.33, 0.33, 0.33});
    simd_float4x4 translateLeft =
        translation_matrix((simd_float3){-1.5, 0.33, -0.75});
    left.set_transform(&left, &scale_left);
    left.set_transform(&left, &translateLeft);
    left.m.color = (Color){1, 0.8, 0.1};
    left.m.diffuse = 0.7;
    left.m.specular = 0.3;
    world->funcTab->add_sphere(world, &left);
    Camera *cam = init_Camera(640 * 2, 640, PI / 3);
    simd_float4 from, to, upVector;
    from = (simd_float4){0, 1.5, -5, 1};
    to = (simd_float4){0, 1, 0, 1};
    upVector = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transMat = view_transform(&from, &to, &upVector);
    *cam->transform = simd_mul(transMat, *cam->transform);
    for (int i = 0; i < world->sphereCounts; ++i)
    {
        printf("%d:\n", i);
        printf("Origin: ");
        print_float4(world->sphereArray[i]->origin);
        print_float3(world->sphereArray[i]->m.color);
        print_float4x4(world->sphereArray[i]->transform);
    }
    PPMCanvas *fig = cam->funcTab->render(cam, world);
    fig->printCanvas(fig, "world_render.ppm");
    system("convert world_render.ppm $PNG_PARAM world_render.png");
    cam->funcTab->destroy(cam);
    world->funcTab->destroy(world);
    fig->destroy(fig);
}

int main(int argc, char *argv[])
{
    render_test();
    return 0;
}
