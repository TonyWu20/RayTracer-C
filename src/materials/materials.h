#pragma once
#include <colors/colors.h>
#include <simd/simd.h>

typedef struct Material Material;
struct Material
{
    Color color;
    float ambient;   // 0 < x <= 1
    float diffuse;   // 0 < x <= 1
    float specular;  // 0 < x <= 1
    float shininess; // usually 10 (large highlight) to 200 (very small) or
                     // higher val
};

/* @abstract: Create a material from input parameters */
static inline Material create_material(Color color, float ambient,
                                       float diffuse, float specular,
                                       float shininess);
/* @abstract: Returns a default Material
 * @returns: A Material
 */
static inline Material defaultMaterial(void);

#pragma mark -Implementations
static inline Material create_material(Color color, float ambient,
                                       float diffuse, float specular,
                                       float shininess)
{
    Material result = {color, ambient, diffuse, specular, shininess};
    return result;
}
static inline Material defaultMaterial(void)
{
    Material result =
        create_material((simd_float3){1, 1, 1}, 0.1, 0.9, 0.9, 200);
    return result;
}
