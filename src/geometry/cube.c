#include <geometry/cube.h>

struct Shape_funcTab cube_funcTab = {
    Cube_intersect_with_ray,
    Cube_set_transform,
    Cube_surface_normal_at,
    Cube_destroy,
};
Cube *create_Cube(void)
{
    Cube *self = malloc(sizeof(Cube));
    self->shape = create_Shape();
    self->shape.funcTab = &cube_funcTab;
    return self;
}
static void check_axis(float origin_xyz, float direction_xyz, float *tmin,
                       float *tmax)
{
    float tmin_numerator = (-1 - origin_xyz);
    float tmax_numerator = (1 - origin_xyz);
    if (fabsf(direction_xyz) >= 10 * EPSILON)
    {
        *tmin = tmin_numerator / direction_xyz;
        *tmax = tmax_numerator / direction_xyz;
    }
    else
    {
        *tmin = tmin_numerator * INFINITY;
        *tmax = tmax_numerator * INFINITY;
    }
    if (*tmin > *tmax)
    {
        float tmp = *tmin;
        *tmin = *tmax;
        *tmax = tmp;
    }
    return;
}
static float max_3(float a, float b, float c)
{
    float max;
    max = (a > b) ? a : b;
    max = (max > c) ? max : c;
    return max;
}
static float min_3(float a, float b, float c)
{
    float min;
    min = (a < b) ? a : b;
    min = (min < c) ? min : c;
    return min;
}
void Cube_intersect_with_ray(Shape *CubeSelf, Ray *r)
{
    float xtmin, xtmax;
    check_axis(r->origin.x, r->directionVec.x, &xtmin, &xtmax);
    float ytmin, ytmax;
    check_axis(r->origin.y, r->directionVec.y, &ytmin, &ytmax);
    float ztmin, ztmax;
    check_axis(r->origin.z, r->directionVec.z, &ztmin, &ztmax);
    float tmin = max_3(xtmin, ytmin, ztmin);
    float tmax = min_3(xtmax, ytmax, ztmax);
    if (!r->xs)
        r->xs = init_intxnCollection();
    if (tmin == INFINITY || tmax == -INFINITY)
        return;
    int *count = &r->xs->intersects_counts;
    *count += 2;
    r->xs->intersects =
        realloc(r->xs->intersects, (*count) * sizeof(Intersect *));
    r->xs->intersects[*count - 2] = mark_intersect(tmin, CubeSelf);
    r->xs->intersects[*count - 1] = mark_intersect(tmax, CubeSelf);
}
void Cube_set_transform(Shape *CubeSelf, Matrix_4x4 *transformMatPtr)
{
    *CubeSelf->transform = simd_mul(*transformMatPtr, *CubeSelf->transform);
}
Vector Cube_surface_normal_at(const Shape *CubeSelf, Point *worldPoint)
{
    Point localPoint =
        simd_mul(simd_inverse(*CubeSelf->transform), *worldPoint);
    float abs_x = fabsf(localPoint.x);
    float abs_y = fabsf(localPoint.y);
    float abs_z = fabsf(localPoint.z);
    float maxc = max_3(abs_x, abs_y, abs_z);
    if (maxc == abs_x)
        return make_Vector(localPoint.x, 0, 0);
    if (maxc == abs_y)
        return make_Vector(0, localPoint.y, 0);
    return make_Vector(0, 0, localPoint.z);
}
void Cube_destroy(Shape *CubeSelf)
{
    Shape_destroy(CubeSelf);
    free(CubeSelf);
}
