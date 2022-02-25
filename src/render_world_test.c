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
    base->material->color = (Color){0.2, 0.2, 0.2};
    base->material->specular = 0.8;
    base->material->ambient = 0.9;
    base->material->reflective = .05;
    Matrix_4x4 scale = scaling_matrix(.5, .5, .5);
    base->material->pattern = (Pattern *)ring_pattern((Color){.4, .08, 0.235},
                                                      (Color){0.1, 0.2, 0.25});
    base->material->pattern->funcTab->set_transform(base->material->pattern, 1,
                                                    &scale);
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
    base->material->reflective = 0.3;
    base->material->pattern =
        (Pattern *)checker_pattern((Color){1, 1, 1}, (Color){0, 0, 0});
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
    base->material->pattern = (Pattern *)stripe_pattern();
    return wall;
}
static Plane *left_wall_create(void)
{
    Plane *wall = create_Plane();
    Shape *base = (Shape *)wall;
    Matrix_4x4 rotate_to_xy = rotation_matrix(PI / 2, (VectorXYZ){1, 0, 0});
    Matrix_4x4 rotate_around_z = rotation_matrix(PI / 3, (VectorXYZ){0, 0, 1});
    Matrix_4x4 translate = translation_matrix(0, 0, 10);
    Shape_set_transform(base, 3, &rotate_around_z, &rotate_to_xy, &translate);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    base->material->diffuse = 0.1;
    base->material->reflective = 0.3;
    base->material->pattern = (Pattern *)ring_pattern((Color){0.86, 0.62, 0.08},
                                                      (Color){0.2, .2, .2});
    Pattern *pat = base->material->pattern;
    Matrix_4x4 scale_pat = scaling_matrix(.5, 1, .5);
    pat->funcTab->set_transform(pat, 1, &scale_pat);
    return wall;
}
static Plane *right_wall_create(void)
{
    Plane *wall = create_Plane();
    Shape *base = (Shape *)wall;
    Matrix_4x4 rotate_to_xy = rotation_matrix(PI / 2, (VectorXYZ){1, 0, 0});
    Matrix_4x4 rotate_around_z = rotation_matrix(-PI / 3, (VectorXYZ){0, 0, 1});
    Matrix_4x4 translate = translation_matrix(0, 0, 10);
    base->material->color = (Color){1, 0.9, 0.9};
    base->material->specular = 0.1;
    base->material->diffuse = 0.5;
    base->material->reflective = 0.2;
    Shape_set_transform(base, 3, &rotate_around_z, &rotate_to_xy, &translate);
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
    Matrix_4x4 scale_ring = scaling_matrix(.5, 1, .1);
    Matrix_4x4 rotate_y = rotation_matrix(-PI / 2, (VectorXYZ){0, -1, 1});
    Pattern *middlePat = (Pattern *)ring_pattern(
        (Color){221.0 / 255, 160.0 / 255, 22.0 / 255}, (Color){0.2, 0.2, 0.2});
    middlePat->funcTab->set_transform(middlePat, 2, &scale_ring, &rotate_y);
    middle->material->pattern = middlePat;
    middle->material->reflective = 0.3;
    // Modify the smaller sphere
    simd_float4x4 translateRight =
        translation_matrix((simd_float3){1.5, 0.5, -0.5});
    Shape *right = world->shapeArray[1];
    right->funcTab->set_transform(right, &translateRight);
    right->material->color = (Color){0.3, 0.3, .7};
    right->material->diffuse = 0.9;
    right->material->specular = 0.9;
    right->material->shininess = 10;
    right->material->reflective = 0.5;
    right->material->pattern =
        (Pattern *)checker_pattern((Color){0.2, 0.2, .8}, (Color){.6, 1, 0});
    Matrix_4x4 scale_pat = scaling_matrix(.3, .3, .3);
    Matrix_4x4 rotate_pat = rotation_matrix(PI / 4, (VectorXYZ){0, 0, 1});
    right->material->pattern->funcTab->set_transform(
        right->material->pattern, 2, &scale_pat, &rotate_pat);
    // Smallest sphere on the left
    Sphere *left = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 scale_left = scaling_matrix((simd_float3){0.33, 0.33, 0.33});
    simd_float4x4 translateLeft =
        translation_matrix((simd_float3){-1.5, 0.33, -0.75});
    Shape *leftBase = (Shape *)left;
    leftBase->funcTab->set_transform(leftBase, &scale_left);
    leftBase->funcTab->set_transform(leftBase, &translateLeft);
    leftBase->material->color = (Color){1, 0.5, 0.5};
    leftBase->material->diffuse = 0.9;
    leftBase->material->specular = 0.9;
    leftBase->material->reflective = 0.3;
    leftBase->material->shininess = 10;
    world->funcTab->add_object(world, leftBase);
    // Add sphere
    Sphere *large = create_Sphere();
    Shape *largeBase = (Shape *)large;
    largeBase->material->color = (Color){.2, .7, .9};
    largeBase->material->diffuse = 0.3;
    largeBase->material->reflective = 0.8;
    Matrix_4x4 translate = translation_matrix(0, 4, 8);
    Matrix_4x4 scale_large = scaling_matrix(2, 2, 2);
    Shape_set_transform(largeBase, 2, &scale_large, &translate);
    world->funcTab->add_object(world, largeBase);
    Camera *cam = init_Camera(1920, 1080, PI / 3);
    simd_float4 from, to, upVector;
    from = (simd_float4){0, 1.5, -5, 1};
    to = (simd_float4){0, 1, 0, 1};
    upVector = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transMat = view_transform(&from, &to, &upVector);
    *cam->transform = simd_mul(transMat, *cam->transform);
    Camera *termCam = init_Camera(140 / 2, 36, PI / 3);
    *termCam->transform = simd_mul(transMat, *termCam->transform);
    printf("\e[?25l");
    /* int prog = 0; */
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    /* world->lights[0]->pos.y = x_y_poly(world->lights[0]->pos.x); */
    printf("\x1b[2J"
           "\x1b[H"); // Clear screen
    PPMCanvas *fig = cam->funcTab->render(cam, world);
    char *frameName;
    asprintf(&frameName, "./render_world/world_render_%i.ppm", 0);
    fig->printCanvas(fig, frameName);
    fig->destroy(fig);
    /* printProgress(prog, 200, (double)(prog) / (double)200, frameName); */
    free(frameName);
    for (int frame = 0; frame < 200; frame++)
    {
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
