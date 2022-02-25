#pragma once
// clang-format off
#include <stdarg.h>
#include <stdlib.h>
#include <geometry/shape.h>
/* #include <types/common.h> */
// clang-format on
struct Pattern
{
    Matrix_4x4 *transform;
    struct Pattern_funcTab *funcTab;
    Color color_a;
    Color color_b;
};

struct Pattern_funcTab
{
    Color (*pattern_at_object)(Pattern *self, const Shape *object,
                               Point *worldPoint);
    Color (*pattern_at)(Pattern *self, Point *objPoint);
    void (*set_transform)(Pattern *self, int transNum, ...);
    void (*destroy)(Pattern *self);
};

/* Common */
/* @abstract: Create a Pattern pointer */
Pattern *create_pattern(Color a, Color b, struct Pattern_funcTab *vtable);
/* @abstract: Determine the color of points at object space */
Color Pattern_at_object(Pattern *self, const Shape *object, Point *worldPoint);
/* @abstract: Apply a series of transformation to the pattern */
/* @params: Pattern *self, int transNum, Matrix_4x4 ... */
static inline void Pattern_set_transform(Pattern *self, int transNum, ...);
void Pattern_destroy(Pattern *self);

/* Specific */
/* @abstract: Pattren stripe */
/* @abstract: Define a stripe of Color a and b*/
FUNC Pattern *stripe_pattern(Color a, Color b);
/* @abstract: default black and white stripe */
FUNC Pattern *stripe_pattern(void);
/* @abstract: Define a gradient color pattern from Color a to Color b */
FUNC Pattern *gradient_pattern(Color a, Color b);
/* @abstract: Define a Ring pattern */
FUNC Pattern *ring_pattern(Color a, Color b);
/* @abstract: Define a Checkboard pattern */
FUNC Pattern *checker_pattern(Color a, Color b);
/* @abstract: Determine the color of points at stripe pattern space */
Color stripe_at(Pattern *self, Point *objPoint);
/* @abstract: Determine the color at gradient pattern space */
Color gradient_at(Pattern *self, Point *objPoint);
/* @abstract: Determine the color at Ring pattern space */
Color ring_at(Pattern *self, Point *objPoint);
/* @abstract: Determine the color at Checkboard pattern space */
Color check_at(Pattern *self, Point *objPoint);

#pragma mark -Implementations

static inline void Pattern_set_transform(Pattern *self, int transNum, ...)
{
    va_list args;
    va_start(args, transNum);
    for (int i = 0; i < transNum; ++i)
    {
        *self->transform =
            simd_mul(*va_arg(args, Matrix_4x4 *), *self->transform);
    }
    va_end(args);
}
