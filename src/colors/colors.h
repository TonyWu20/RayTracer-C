#pragma once
#include <simd/simd.h>

typedef simd_float3 Color; // typedef a simd_float3 as Color

static inline int lumaForColor(Color result)
{
    simd_float3 luminanceFactor = {0.2126, 0.7152, 0.0722};
    result = simd_clamp(result, 0, 1);
    float lum = simd_dot(luminanceFactor, result);
    int lumN = 11 * lum;
    return lumN;
}
