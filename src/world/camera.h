#pragma once
#include <canvas/canvas.h>
#include <canvas/ppm_canvas.h>
#include <geometry/geometry.h>
#include <ray/ray.h>
#include <stdlib.h>
#include <types/types.h>
#include <world/world.h>
/* typedef */
typedef struct camara Camera;
struct camara
{
    int hSize;
    int vSize;
    float fovAngle;
    float half_width;
    float half_height;
    float pixelSize;
    Matrix_4x4 *transform;
    struct Camera_VTable *funcTab;
};

struct Camera_VTable
{
    void (*destroy)(Camera *);
    Ray (*ray_for_pixel)(Camera *, int, int);
    PPMCanvas *(*render)(Camera *, World *);
    Canvas *(*ascii_render)(Camera *, World *);
};
/* typedef ends */

/* Functions prototypes */
/* @abstract: Returns a pointer of Camera defined by hSize, vSize and fov angle
 * @params: int hSize, int vSize, float fovAngle
 * @Warning: The return pointer, and the matrix pointer is malloced.
 */
static inline Camera *init_Camera(int hSize, int vSize, float fovAngle);
/* @abstract: Calculate pixelSize on the canvas */
/* @params: Camera *self */
static inline void compute_pixelSize(Camera *self);
/* @abstract: Free the memory malloced to simd_float4x4 *transform and Camera
 * *self
 */
static inline void destroy_Camera(Camera *self);
/* @abstract: Returns a new ray that starts at at the camera and */
/* passes through the indicated (x, y) pixel on the canvas. */
static inline Ray ray_for_pixel(Camera *self, int x, int y);
/* @abstract: Render an image of the given world */
/* @params: Camera *self, World *world */
/* @Returns: PPMCanvas *fig */
/* @Warning: The returned PPMCanvas *fig is malloced */
static inline PPMCanvas *render(Camera *self, World *world);
/* @abstract: Render the given world to terminal buffer */
/* @params: Camera *self, World *world */
/* @Returns: Canvas *fig */
/* @Warning: The returned Canvas *fig is malloced */
static inline Canvas *renderASCII(Camera *self, World *world);

#pragma mark -Implementations
struct Camera_VTable camVTable = {
    destroy_Camera,
    ray_for_pixel,
    render,
    renderASCII,
};
static inline Camera *init_Camera(int hSize, int vSize, float fovAngle)
{
    Camera *cam = malloc(sizeof(Camera));
    cam->hSize = hSize;
    cam->vSize = vSize;
    cam->fovAngle = fovAngle;
    cam->transform = malloc(sizeof(Matrix_4x4));
    *cam->transform = matrix_identity_float4x4;
    cam->funcTab = &camVTable;
    compute_pixelSize(cam); // assign half_width/height and pixelSize
    return cam;
}
static inline void compute_pixelSize(Camera *self)
{
    float half_view = tan(self->fovAngle / 2);
    float aspect = (float)self->hSize / (float)self->vSize;
    if (aspect >= 1)
    {
        self->half_width = half_view;
        self->half_height = half_view / aspect;
    }
    else
    {
        self->half_width = half_view * aspect;
        self->half_height = half_view;
    }
    self->pixelSize = (self->half_width * 2) / self->hSize;
}
static inline void destroy_Camera(Camera *self)
{
    free(self->transform);
    free(self);
}
static inline Ray ray_for_pixel(Camera *self, int x, int y)
{
    // The offset from the edge of the canvas to the pixel's center
    float xoffset = (x + 0.5) * self->pixelSize;
    float yoffset = (y + 0.5) * self->pixelSize;
    // The untransformed coordinates of the pixel in world space.
    // (remember that the camera looks toward -z, so +x is to *right*)
    // *Different from the book since we're using a right-hand coord
    float world_x = self->half_width - xoffset; // +x to right
    float world_y = self->half_height - yoffset;
    // using the camera matrix, transform the canvas point and the origin,
    // then compute the ray's direction vector.
    // remember the canvas is at z = -1;
    Point pixel = simd_mul(simd_inverse(*self->transform),
                           (Point){world_x, world_y, -1, 1});
    Point origin =
        simd_mul(simd_inverse(*self->transform), (Point){0, 0, 0, 1});
    Vector directionVec = simd_normalize(pixel - origin);
    return init_Ray(origin, directionVec);
}
static inline PPMCanvas *render(Camera *self, World *world)
{
    PPMCanvas *fig = init_PPMCanvas(self->hSize, self->vSize);
    for (int y = 0; y < self->vSize; ++y)
    {
        for (int x = 0; x < self->hSize; ++x)
        {
            Ray r = ray_for_pixel(self, x, y);
            Color color = color_at(world, &r);
            fig->writePixel(fig, x, y, color);
            r.destroy_XS(&r);
        }
    }
    return fig;
}
static inline Canvas *renderASCII(Camera *self, World *world)
{
    Canvas *fig = init_Canvas(2 * self->hSize, self->vSize);
    // clang-format off
    // clang-format on
    for (int y = 0; y < self->vSize; ++y)
    {
        for (int x = 0; x < self->hSize; ++x)
        {
            Ray r = ray_for_pixel(self, x, y);
            Color color = color_at(world, &r);
            fig->writeLumaPixel(fig, 2 * x, y, color);
            fig->writeLumaPixel(fig, 2 * x + 1, y, color);
            r.destroy_XS(&r);
        }
    }
    return fig;
}
