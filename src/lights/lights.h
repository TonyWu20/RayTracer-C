#pragma once
#include <simd/simd.h>

typedef struct Light Light;
typedef simd_float3 Color; // typedef a simd_float3 as Color

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

#pragma mark -Implementations
static inline Light point_light(simd_float4 pos, Color intensity)
{
    Light result;
    result.pos = pos;
    result.intensity = intensity;
    return result;
}
