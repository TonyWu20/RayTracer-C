#pragma once
#include <types/types.h>
#include <stdio.h>

static inline int lumaForColor(Color result)
{
    simd_float3 luminanceFactor = {0.2126, 0.7152, 0.0722};
    result = simd_clamp(result, 0, 1);
    float lum = simd_dot(luminanceFactor, result);
    int lumN = 11 * lum;
    return lumN;
}
static inline char lumaToChar(Color result)
{
    simd_float3 luminanceFactor = {0.2126, 0.7152, 0.0722};
    result = simd_clamp(result, 0, 1);
    float lum = simd_dot(luminanceFactor, result);
    char grayscale[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
                       "\\|()1{}[]?-_+~<>i!lI;:,\"^`' ";
    int lumN = 68 * lum;
    return grayscale[68 - lumN];
}
