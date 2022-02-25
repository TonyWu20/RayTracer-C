#include <geometry/patterns.h>
#include <geometry/shape.h>

const Color black = {0, 0, 0};
const Color white = {1, 1, 1};

struct Pattern_funcTab stripe_funcTab = {
    Pattern_at_object,
    stripe_at,
    Pattern_set_transform,
    Pattern_destroy,
};

struct Pattern_funcTab gradient_funcTab = {
    Pattern_at_object,
    gradient_at,
    Pattern_set_transform,
    Pattern_destroy,
};

struct Pattern_funcTab ring_funcTab = {
    Pattern_at_object,
    ring_at,
    Pattern_set_transform,
    Pattern_destroy,
};

struct Pattern_funcTab check_funcTab = {
    Pattern_at_object,
    check_at,
    Pattern_set_transform,
    Pattern_destroy,
};
/* Stripe */
Pattern *create_pattern(Color a, Color b, struct Pattern_funcTab *vtable)
{
    Pattern *self = malloc(sizeof(Pattern));
    self->transform = malloc(sizeof(Matrix_4x4));
    *self->transform = matrix_identity_float4x4;
    self->funcTab = vtable;
    self->color_a = a;
    self->color_b = b;
    return self;
}
FUNC Pattern *stripe_pattern(Color a, Color b)
{
    return create_pattern(a, b, &stripe_funcTab);
}
FUNC Pattern *stripe_pattern(void)
{
    return stripe_pattern(white, black);
}

Color stripe_at(Pattern *self, Point *objPoint)
{
    float x = objPoint->x;
    if ((int)x % 2 == 0)
    {
        return self->color_a;
    }
    else
    {
        return self->color_b;
    }
}
/* Gradient */
FUNC Pattern *gradient_pattern(Color a, Color b)
{
    return create_pattern(a, b, &gradient_funcTab);
}
Color gradient_at(Pattern *self, Point *objPoint)
{
    Color distance = self->color_b - self->color_a;
    float fraction = objPoint->x - floorf(objPoint->x);
    return self->color_a + distance * fraction;
}
/* Ring */
FUNC Pattern *ring_pattern(Color a, Color b)
{
    return create_pattern(a, b, &ring_funcTab);
}
Color ring_at(Pattern *self, Point *objPoint)
{
    float px, pz;
    px = objPoint->x;
    pz = objPoint->z;
    if ((int)floorf((sqrtf(px * px + pz * pz)) + 100 * EPSILON) % 2 == 0)
        return self->color_a;
    else
        return self->color_b;
}
/* Check */
FUNC Pattern *checker_pattern(Color a, Color b)
{
    return create_pattern(a, b, &check_funcTab);
}
Color check_at(Pattern *self, Point *objPoint)
{
    *objPoint =
        *objPoint + (Point){100 * EPSILON, 100 * EPSILON, 100 * EPSILON, 0};
    int floor_sum =
        floor(objPoint->x) + floor(objPoint->y) + floor(objPoint->z);
    if (floor_sum % 2 == 0)
        return self->color_a;
    else
        return self->color_b;
}
void Pattern_destroy(Pattern *self)
{
    free(self->transform);
    free(self);
}
Color Pattern_at_object(Pattern *self, const Shape *object, Point *worldPoint)
{
    Point objPoint = simd_mul(simd_inverse(*object->transform), *worldPoint);
    Point patternPoint = simd_mul(simd_inverse(*self->transform), objPoint);
    return self->funcTab->pattern_at(self, &patternPoint);
}
