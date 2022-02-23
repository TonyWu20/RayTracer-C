#include <main.h>
#include <signal.h>
#define FRAMERATE 240
#define SLEEPTIME (1000 / FRAMERATE) % 1000 * 1000000;

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}

static inline void castRayOnSphere(float half, float wall_z, float pixel_size,
                                   simd_float4 ray_origin, Sphere *s,
                                   Light *light, Canvas *fig)
{
    int shift_x = (fig->width - 2 * fig->height) / 2;
    for (int y = 0; y < fig->height; ++y)
    {
        for (int x = 0; x < fig->width; ++x)
        {
            float world_y = half - pixel_size * y;
            float world_x = -half + pixel_size * x;
            simd_float4 pos = {world_x, world_y, wall_z, 1};
            Ray r = init_Ray(ray_origin, simd_normalize(pos - ray_origin));
            r.intersects_with_Sphere(&r, s);
            IntersectCollection *xs = r.xs;
            if (xs)
            {
                SphereIntersect *hit = hit_Sphere(xs);
                simd_float4 hitPoint = currPosition(&r, hit->t);
                simd_float4 surfaceNormal =
                    hit->object->surface_normal_at(hit->object, &hitPoint);
                simd_float4 eye = -r.directionVec;
                Color pixelColor =
                    lighting(&s->m, light, &hitPoint, &eye, &surfaceNormal);
                int luma = lumaForColor(pixelColor);
                fig->writeLumaPixel(fig, shift_x + 2 * x, y, luma);
                fig->writeLumaPixel(fig, shift_x + 2 * x - 1, y, luma);
                xs->destroy(xs);
            }
        }
    }
}
static inline void castRayOnSphere_PPM(float half, float wall_z,
                                       float pixel_size, simd_float4 ray_origin,
                                       Sphere *s, Light *light, PPMCanvas *fig)
{
    /* int shift_x = (fig->width - 2 * fig->height) / 2; */
    for (int y = 0; y < fig->height; ++y)
    {
        for (int x = 0; x < fig->width; ++x)
        {
            float world_y = half - pixel_size * y;
            float world_x = -half + pixel_size * x;
            simd_float4 pos = {world_x, world_y, wall_z, 1};
            Ray r = init_Ray(ray_origin, simd_normalize(pos - ray_origin));
            r.intersects_with_Sphere(&r, s);
            IntersectCollection *xs = r.xs;
            if (xs)
            {
                SphereIntersect *hit = hit_Sphere(xs);
                simd_float4 hitPoint = currPosition(&r, hit->t);
                simd_float4 surfaceNormal =
                    hit->object->surface_normal_at(hit->object, &hitPoint);
                simd_float4 eye = -1 * r.directionVec;
                Color pixelColor =
                    lighting(&s->m, light, &hitPoint, &eye, &surfaceNormal);
                fig->writePixel(fig, x, y, pixelColor);
                xs->destroy(xs);
            }
        }
    }
}
void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    exit(0);
}
void silhouette()
{
    float r = 1;
    Sphere s = create_Sphere((simd_float4){0, 0, 0, 1}, r);
    s.m.color = (Color){1, 0.8, 1};
    simd_float4 ray_origin = {0, 0, -2, 1};
    float wall_z = 10;
    float on_wall_d = 2 * r * (wall_z - ray_origin.z) / (0.0 - ray_origin.z);
    float wall_size = on_wall_d + 2;
    Light light =
        point_light((simd_float4){-10, -10, -10, 1}, (Color){1, 1, 1});
    /* WINDOW *win = initscr(); */
    /* int row, col; */
    /* getmaxyx(win, row, col); */
    int row = 48;
    int col = 177;
    float half = wall_size / 2;
    float pixel_size = wall_size / row;
    printf("\e[?25l");
    Canvas *fig = init_Canvas(col, row);
    PPMCanvas *ppmFig = init_PPMCanvas(col, row);
    castRayOnSphere_PPM(half, wall_z, pixel_size, ray_origin, &s, &light,
                        ppmFig);
    ppmFig->printCanvas(ppmFig, "./ray_test.ppm");
    ppmFig->destroy(ppmFig);
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = SLEEPTIME;
    while (light.pos.x <= 10)
    {
        light.pos.x += 0.2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.y <= 10)
    {
        light.pos.y += 0.2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.x >= -10)
    {
        light.pos.x -= 0.2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.y >= -10)
    {
        light.pos.y -= 0.2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.x <= 0 && light.pos.y <= 0)
    {
        light.pos += (simd_float4){0.1, 0.1, 0, 0};
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.z > -15)
    {
        light.pos.z -= .2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.z <= -1)
    {
        light.pos.z += .2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    while (light.pos.z > -10)
    {
        light.pos.z -= .2;
        castRayOnSphere(half, wall_z, pixel_size, ray_origin, &s, &light, fig);
        fig->show(fig);
        nanosleep(&tim, &tim2);
    }
    fig->destroy(fig);
    printf("\e[?25h");
    /* refresh(); */
    /* getch(); */
    /* endwin(); */
    /* delwin(win); */
}

void silhouette_image()
{
    float r = 1;
    Sphere s = create_Sphere((simd_float4){0, 0, 0, 1}, r);
    s.m.color = (Color){22.0 / 255, 252.0 / 255, 122.0 / 255};
    s.m.shininess = 10;
    s.m.diffuse = 0.9;
    s.m.specular = .9;
    s.m.ambient = .12;
    simd_float4x4 scale_half = scaling_matrix((simd_float3){0.5, 0.5, 0.5});
    s.set_transform(&s, &scale_half);
    simd_float4 ray_origin = {0, 0, -5, 1};
    float wall_z = 10;
    float on_wall_d = 2 * r * (wall_z - ray_origin.z) / (0.0 - ray_origin.z);
    float wall_size = on_wall_d + 2;
    Light light =
        point_light((simd_float4){-10, -10, -10, 1}, (Color){1, 1, 1});
    /* WINDOW *win = initscr(); */
    /* int row, col; */
    /* getmaxyx(win, row, col); */
    int row = 1024;
    int col = 1024;
    float half = wall_size / 2;
    float pixel_size = wall_size / row;
    PPMCanvas *ppmFig = init_PPMCanvas(col, row);
    castRayOnSphere_PPM(half, wall_z, pixel_size, ray_origin, &s, &light,
                        ppmFig);
    ppmFig->printCanvas(ppmFig, "./ray_test.ppm");
    ppmFig->destroy(ppmFig);
    system("convert ray_test.ppm $PNG_PARAM ray_test.png");
    printf("Done!");
    /* refresh(); */
    /* getch(); */
    /* endwin(); */
    /* delwin(win); */
}
void silhouette_image2()
{
    World *w = init_default_world();
    Camera *cam = init_Camera(1024, 1024, PI / 2);
    Sphere *s0 = w->sphereArray[0];
    Sphere *s1 = w->sphereArray[1];
    simd_float4x4 translate_s0 =
        translation_matrix((simd_float3){-0.5, 1, 0.5});
    simd_float4x4 translate_s1 =
        translation_matrix((simd_float3){1.5, 0.5, -0.5});
    s0->set_transform(s0, &translate_s0);
    s0->m.color = (Color){1, 1, 1};
    s1->set_transform(s1, &translate_s1);
    // Floor
    Sphere floor = create_Sphere((simd_float4){0, 0, 0, 1}, 1);
    simd_float4x4 flatten_to_wall = scaling_matrix((simd_float3){10, 0.01, 6});
    floor.set_transform(&floor, &flatten_to_wall);
    floor.m.color = (Color){1, 0.9, 0.9};
    floor.m.specular = 0.1;
    w->funcTab->add_sphere(w, &floor);
    // Floor ends
    simd_float4 from, to, up;
    from = (simd_float4){0, 1.5, -5, 1};
    to = (simd_float4){0, 1, 0, 1};
    up = (simd_float4){0, 1, 0, 0};
    simd_float4x4 transform = view_transform(&from, &to, &up);
    *cam->transform = transform;
    PPMCanvas *fig = cam->funcTab->render(cam, w);
    fig->printCanvas(fig, "ray_test2.ppm");
    system("convert ray_test2.ppm $PNG_PARAM ray_test2.png");
    fig->destroy(fig);
    w->funcTab->destroy(w);
    cam->funcTab->destroy(cam);
}
int main(int argc, char *argv[])
{
    signal(SIGINT, sig_handler);
    time_t t = clock();
    silhouette_image();
    t = clock() - t;
    double timeInterval = (double)t / CLOCKS_PER_SEC;
    printf("Render ppm took %f sec.\n", timeInterval);
    silhouette_image2();
    /* silhouette(); */
    /* test_intersect(); */
    return 0;
}
