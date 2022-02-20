#pragma once
#include <canvas/canvas.h>
#include <geometry/geometry.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* @abstract: Quick print of a float4 vector */
static inline void print_float4(simd_float4 vec4);

#pragma mark -Implementation
static inline void print_float4(simd_float4 vec4)
{
    printf("(%f, %f, %f, %f)\n", vec4.x, vec4.y, vec4.z, vec4.w);
}
