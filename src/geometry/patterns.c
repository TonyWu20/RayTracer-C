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
/* Stripe */
FUNC Pattern *stripe_pattern(Color a, Color b)
{
    Pattern *self = malloc(sizeof(Pattern));
    self->transform = malloc(sizeof(Matrix_4x4));
    *self->transform = matrix_identity_float4x4;
    self->funcTab = &stripe_funcTab;
    self->color_a = a;
    self->color_b = b;
    return self;
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
    Pattern *self = malloc(sizeof(Pattern));
    self->transform = malloc(sizeof(Matrix_4x4));
    *self->transform = matrix_identity_float4x4;
    self->funcTab = &gradient_funcTab;
    self->color_a = a;
    self->color_b = b;
    return self;
}
Color gradient_at(Pattern *self, Point *objPoint)
{
    Color distance = self->color_b - self->color_a;
    float fraction = objPoint->x - floorf(objPoint->x);
    return self->color_a + distance * fraction;
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
