#pragma once
#include <colors/colors.h>
#include <main.h>
#include <materials/materials.h>
#include <simd/simd.h>
#include <stdio.h>

typedef struct Light Light;

struct Light
{
    Color intensity;
    simd_float4 pos;
};

/* @abstract: Create a Light that has a position and intensity
 * @params: simd_float4 pos, Color intensity
 * @returns: a Light struct
 */
static inline Light point_light(simd_float4 pos, Color intensity);

/* @abstract: Shade the object by the material, light, point being illuminated,
 * eye and normal vectors from the Phong reflection model
 * @returns: a Color type
 */
static inline Color lighting(Material *m, Light *light, simd_float4 *point,
                             simd_float4 *eyeV, simd_float4 *normalV);

#pragma mark -Implementations
static inline Light point_light(simd_float4 pos, Color intensity)
{
    Light result;
    result.pos = pos;
    result.intensity = intensity;
    return result;
}

static inline Color lighting(Material *m, Light *light, simd_float4 *point,
                             simd_float4 *eyeV, simd_float4 *normalV)
{
    /* Combine the surface color with the light's intensity */
    Color eff_color = m->color * light->intensity;
    /* Find the direction to the light source */
    simd_float4 lightV = simd_normalize(light->pos - *point);
    /* Compute the ambient contribution */
    Color ambient = m->ambient * eff_color;
    /* light_dot_normal represents the cosine of the angle
     * between the light vector and the normal vector.
     * < 0 means the light is on the other side of the surface
     */
    float light_dot_normal = simd_dot(lightV, *normalV);
    /* Contribution to final color computation */
    Color diffuse, specular;
    if (light_dot_normal < 0) // Light is on the other side
    {
        diffuse = (simd_float3){0, 0, 0};
        specular = (simd_float3){0, 0, 0};
    }
    else
    {
        /* diffuse contribution */
        diffuse = eff_color * m->diffuse * light_dot_normal;
        /* reflect_dot_eye represents the cosine of the angle
         * between the reflection vector and the eye vector.
         * A negative number means the light reflects away from the eye.
         */
        simd_float4 reflectV = simd_reflect(
            lightV,
            *normalV); // The normalV from surface_normal_at is normalized
        float reflect_dot_eye = simd_dot(reflectV, *eyeV);
        if (reflect_dot_eye <= 0)
            specular = (simd_float3){0, 0, 0};
        else
        {
            float factor = pow(reflect_dot_eye, m->shininess);
            specular = light->intensity * m->specular * factor;
        };
    }
    return ambient + diffuse + specular;
}
