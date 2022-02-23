#pragma once
#include <geometry/geometry.h>
#include <ray/ray.h>
#include <stdlib.h>
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
    simd_float4x4 *transform;
    struct Camera_VTable *funcTab;
};

struct Camera_VTable
{
    void (*destroy)(Camera *);
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

#pragma mark -Implementations
struct Camera_VTable camVTable = {
    destroy_Camera,
};
static inline Camera *init_Camera(int hSize, int vSize, float fovAngle)
{
    Camera *cam = malloc(sizeof(Camera));
    cam->hSize = hSize;
    cam->vSize = vSize;
    cam->fovAngle = fovAngle;
    cam->transform = malloc(sizeof(simd_float4x4));
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
    float world_x = self->half_width + xoffset; // +x to right
    float world_y = self->half_height - yoffset;
    // using the camera matrix, transform the canvas point and the origin,
    // then compute the ray's direction vector.
    // remember the canvas is at z = -1;
    simd_float4 pixel = simd_mul(simd_inverse(*self->transform),
                                 (simd_float4){world_x, world_y, -1, 1});
    simd_float4 origin =
        simd_mul(simd_inverse(*self->transform), (simd_float4){0, 0, 0, 1});
    simd_float4 directionVec = simd_normalize(pixel - origin);
    return init_Ray(origin, directionVec);
}