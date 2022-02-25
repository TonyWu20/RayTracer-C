#include <main.h>
#include <omp.h>
#define FRAMERATE 60
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;
// The floor is an extremely flattened sphere with a matte texture
static Plane *floor_create(void)
{
    Plane *floor = create_Plane();
    /* simd_float4x4 rotate_to_xy = rotation_matrix(PI/2, (VectorXYZ){1,0,0});
     */
    Shape *base = (Shape *)floor;
    /* base->funcTab->set_transform(base, &rotate_to_xy); */
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    return floor;
}
static Plane *ceiling_create(void)
{
    Plane *floor = create_Plane();
    /* simd_float4x4 rotate_to_xy = rotation_matrix(PI/2, (VectorXYZ){1,0,0});
     */
    Matrix_4x4 translate = translation_matrix(0, 11, 0);
    Shape *base = (Shape *)floor;
    base->funcTab->set_transform(base, &translate);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    return floor;
}
static Plane *wall_create(float around_z, float tz)
{
    Plane *wall = create_Plane();
    Shape *base = (Shape *)wall;
    Matrix_4x4 rotate_to_xy = rotation_matrix(PI / 2, (VectorXYZ){1, 0, 0});
    Matrix_4x4 rotate_around_z =
        rotation_matrix(around_z, (VectorXYZ){0, 0, 1});
    Matrix_4x4 translate = translation_matrix(0, 0, tz);
    Shape_set_transform(base, 3, &rotate_around_z, &rotate_to_xy, &translate);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    return wall;
}
static Plane *left_wall_create(void)
{
    Plane *wall = create_Plane();
    Shape *base = (Shape *)wall;
    Matrix_4x4 rotate_to_xy = rotation_matrix(PI / 2, (VectorXYZ){1, 0, 0});
    Matrix_4x4 rotate_around_z = rotation_matrix(PI / 6, (VectorXYZ){0, 0, 1});
    Matrix_4x4 translate = translation_matrix(0, 0, 10);
    Shape_set_transform(base, 3, &rotate_around_z, &rotate_to_xy, &translate);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    return wall;
}
static Plane *right_wall_create(void)
{
    Plane *wall = create_Plane();
    Shape *base = (Shape *)wall;
    Matrix_4x4 rotate_to_xy = rotation_matrix(PI / 2, (VectorXYZ){1, 0, 0});
    Matrix_4x4 rotate_around_z = rotation_matrix(-PI / 6, (VectorXYZ){0, 0, 1});
    Matrix_4x4 translate = translation_matrix(0, 0, 10);
    Shape_set_transform(base, 3, &rotate_around_z, &rotate_to_xy, &translate);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    return wall;
}
static float x_y_poly(float x)
{
    float y = -(x * x) / 12.5 + 10;
    return y;
}
// Same scale and color of the wall, but rotated and translated

static void render_test(void)
{
    // The floor is an extremely flattened sphere with a matte texture
    Plane *floor = floor_create();
    // Wall on the left
    /* Sphere left_wall = left_wall_create(); */
    // Wall on the right, rotated in opposite direction in y
    /* Sphere right_wall = right_wall_create(); */
    // Create world
    World *world = init_default_world();
    world->lights[0]->intensity *= 1.2;
    // Add floor and walls
    world->funcTab->add_object(world, (Shape *)floor);
    world->funcTab->add_object(world, (Shape *)ceiling_create());
    world->funcTab->add_object(world, (Shape *)left_wall_create());
    world->funcTab->add_object(world, (Shape *)right_wall_create());
    // Modify the large sphere in the middle
    simd_float4x4 translateMiddle =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    Shape *middle = world->shapeArray[0];
    middle->funcTab->set_transform(middle, &translateMiddle);
    middle->material->color = (Color){0.3, 1, 0.5}; // Green
    middle->material->diffuse = 0.8;
    middle->material->specular = 0.99;
    // Modify the smaller sphere
    simd_float4x4 translateRight =
        translation_matrix((simd_float3){1.5, 0.5, -0.5});
    Shape *right = world->shapeArray[1];
    right->funcTab->set_transform(right, &translateRight);
    right->material->color = (Color){0.3, 0.5, 1};
    right->material->diffuse = 0.9;
    right->material->specular = 0.9;
    right->material->shininess = 10;
    // Smallest sphere on the left
    Sphere *left = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 scale_left = scaling_matrix((simd_float3){0.33, 0.33, 0.33});
    simd_float4x4 translateLeft =
        translation_matrix((simd_float3){-1.5, 0.33, -0.75});
    Shape *leftBase = (Shape *)left;
    leftBase->funcTab->set_transform(leftBase, &scale_left);
    leftBase->funcTab->set_transform(leftBase, &translateLeft);
    leftBase->material->color = (Color){1, 0.5, 0.5};
    leftBase->material->diffuse = 0.7;
    leftBase->material->specular = 0.3;
    world->funcTab->add_object(world, leftBase);
    Camera *cam = init_Camera(640, 320, PI / 3);
    simd_float4 from, to, upVector;
    from = (simd_float4){0, 1.5, -5, 1};
    to = (simd_float4){0, 1, 0, 1};
    upVector = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transMat = view_transform(&from, &to, &upVector);
    *cam->transform = simd_mul(transMat, *cam->transform);
    Camera *termCam = init_Camera(88 / 2, 24, PI / 3);
    *termCam->transform = simd_mul(transMat, *termCam->transform);
    printf("\e[?25l");
    int prog = 0;
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    world->lights[0]->pos.y = x_y_poly(world->lights[0]->pos.x);
    printf("\x1b[2J"
           "\x1b[H"); // Clear screen
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
        nanosleep(&tim, &tim2);
        termFig->show(termFig);
        world->lights[0]->pos.x = -10 + (float)frame * 0.1;
        world->lights[0]->pos.y = x_y_poly(world->lights[0]->pos.x);
        termFig->destroy(termFig);
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
