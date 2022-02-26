#pragma once
// clang-format off
#include <colors/colors.h>
#include <types/common.h>
#include <types/types.h>
/* #include <geometry/patterns.h> */
// clang-format on

struct Material
{
    Color color;
    float ambient;   // 0 < x <= 1
    float diffuse;   // 0 < x <= 1
    float specular;  // 0 < x <= 1
    float shininess; // usually 10 (large highlight) to 200 (very small) or
                     // higher val
    float reflective;
    float transparency;
    float refractive_index;
    Pattern *pattern;
};

/* @abstract: Create a material from input parameters */
static inline Material create_material(Color color, float ambient,
                                       float diffuse, float specular,
                                       float shininess, float reflective,
                                       float trans, float refractive,
                                       Pattern *pattern);
/* @abstract: Returns a default Material
 * @returns: A Material
 */
static inline Material defaultMaterial(void);

#pragma mark -Implementations
static inline Material create_material(Color color, float ambient,
                                       float diffuse, float specular,
                                       float shininess, float reflective,
                                       float trans, float refractive,
                                       Pattern *pattern)
{
    Material result = {color,      ambient, diffuse,    specular, shininess,
                       reflective, trans,   refractive, pattern};
    return result;
}
static inline Material defaultMaterial(void)
{
    Color color = {1, 1, 1};
    float ambient = 0.1;
    float diffuse = 0.9;
    float specular = 0.9;
    float shininess = 200;
    float reflective = 0.0;
    float transparency = 0.0;
    float refractive_index = 1.0;
    Pattern *pattern = NULL;
    Material result =
        create_material(color, ambient, diffuse, specular, shininess,
                        reflective, transparency, refractive_index, pattern);
    return result;
}
