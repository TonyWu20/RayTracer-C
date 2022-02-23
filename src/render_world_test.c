#include <main.h>
#include <omp.h>
#define FRAMERATE 24
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;
// The floor is an extremely flattened sphere with a matte texture
static Sphere floor_sphere()
{
    Sphere floor = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 flatten_to_wall = scaling_matrix((simd_float3){10, 0.01, 10});
    floor.set_transform(&floor, &flatten_to_wall);
    floor.m.color = (Color){1, 0.9, 0.9};
    floor.m.specular = 0.1;
    return floor;
}
static float x_y_poly(float x)
{
    float y = -(x * x) / 12.5 + 10;
    return y;
}
// Same scale and color of the wall, but rotated and translated
static Sphere left_wall_create()
{
    Sphere left_wall = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 flatten_to_wall =
        scaling_matrix((simd_float3){100, 0.1, 100});
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
    return left_wall;
}
static Sphere right_wall_create()
{
    Sphere right_wall = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 flatten_to_wall =
        scaling_matrix((simd_float3){100, 0.1, 100});
    right_wall.set_transform(&right_wall, &flatten_to_wall);
    simd_float4x4 rotation_x =
        simd_matrix4x4(simd_quaternion(PI / 2, (simd_float3){1, 0, 0}));
    simd_float4x4 rotation_y =
        simd_matrix4x4(simd_quaternion(PI / 4, (simd_float3){0, 1, 0}));
    simd_float4x4 translateMat = translation_matrix((simd_float3){0, 0, 5});
    right_wall.set_transform(&right_wall, &rotation_x);
    right_wall.set_transform(&right_wall, &rotation_y);
    right_wall.set_transform(&right_wall, &translateMat);
    right_wall.m.color = (Color){1, 0.9, 0.9};
    right_wall.m.specular = 0.1;
    return right_wall;
}

static void render_test(void)
{
    // The floor is an extremely flattened sphere with a matte texture
    Sphere floor = floor_sphere();
    // Wall on the left
    Sphere left_wall = left_wall_create();
    // Wall on the right, rotated in opposite direction in y
    Sphere right_wall = right_wall_create();
    // Create world
    World *world = init_default_world();
    world->lights[0]->intensity *= 1.2;
    // Add floor and walls
    world->funcTab->add_sphere(world, &floor);
    world->funcTab->add_sphere(world, &left_wall);
    world->funcTab->add_sphere(world, &right_wall);
    // Modify the large sphere in the middle
    simd_float4x4 translateMiddle =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    Sphere *middle = world->sphereArray[0];
    middle->set_transform(middle, &translateMiddle);
    middle->m.color = (Color){0.3, 1, 0.5}; // Green
    middle->m.diffuse = 0.8;
    middle->m.specular = 0.99;
    // Modify the smaller sphere
    simd_float4x4 translateRight =
        translation_matrix((simd_float3){1.5, 0.5, -0.5});
    Sphere *right = world->sphereArray[1];
    right->set_transform(right, &translateRight);
    right->m.color = (Color){0.3, 0.5, 1};
    right->m.diffuse = 0.9;
    right->m.specular = 0.9;
    right->m.shininess = 10;
    // Smallest sphere on the left
    Sphere left = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 scale_left = scaling_matrix((simd_float3){0.33, 0.33, 0.33});
    simd_float4x4 translateLeft =
        translation_matrix((simd_float3){-1.5, 0.33, -0.75});
    left.set_transform(&left, &scale_left);
    left.set_transform(&left, &translateLeft);
    left.m.color = (Color){1, 0.5, 0.5};
    left.m.diffuse = 0.7;
    left.m.specular = 0.3;
    world->funcTab->add_sphere(world, &left);
    Camera *cam = init_Camera(640, 320, PI / 3);
    simd_float4 from, to, upVector;
    from = (simd_float4){0, 1.5, -5, 1};
    to = (simd_float4){0, 1, 0, 1};
    upVector = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transMat = view_transform(&from, &to, &upVector);
    *cam->transform = simd_mul(transMat, *cam->transform);
    Camera *termCam = init_Camera(177 / 2, 48, PI / 3);
    *termCam->transform = simd_mul(transMat, *termCam->transform);
    printf("\033[1;33m");
    printf("\e[?25l");
    int prog = 0;
    world->lights[0]->pos.y = x_y_poly(world->lights[0]->pos.x);
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    for (int frame = 0; frame < 200; frame++)
    {
        /* PPMCanvas *fig = cam->funcTab->render(cam, world); */
        /* char *frameName; */
        /* asprintf(&frameName, "./render_world/world_render_%i.ppm", frame); */
        /* fig->printCanvas(fig, frameName); */
        /* fig->destroy(fig); */
        /* prog++; */
        /* printProgress(prog, 200, (double)(prog) / (double)200, frameName); */
        /* free(frameName); */
        Canvas *termFig = termCam->funcTab->ascii_render(termCam, world);
        termFig->show(termFig);
        termFig->destroy(termFig);
        world->lights[0]->pos.x = -10 + (float)frame * 0.1;
        world->lights[0]->pos.y = x_y_poly(world->lights[0]->pos.x);
        nanosleep(&tim, &tim2);
    }
    cam->funcTab->destroy(cam);
    world->funcTab->destroy(world);
    termCam->funcTab->destroy(termCam);
    printf("\033[0m");
    printf("\e[?25h");
}

int main(int argc, char *argv[])
{
    render_test();
    return 0;
}
