#pragma once
#include <canvas/canvas.h>
#include <canvas/ppm_canvas.h>
#include <colors/colors.h>
#include <geometry/geometry.h>
#include <lights/lights.h>
#include <materials/materials.h>
#include <ray/ray.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <world/camera.h>
#include <world/world.h>
#define PI (atan(1) * 4)

/* @abstract: Quick print of a float4 vector */
static inline void print_float4(simd_float4 vec4);
/* @abstract: Quick print of a float3 vector */
static inline void print_float3(simd_float3 vec3);
/* @abstract: Pretty print of a float4x4 matrix */
static inline void print_float4x4(simd_float4x4 mat);

#pragma mark -Implementation
static inline void print_float4(simd_float4 vec4)
{
    printf("(%f, %f, %f, %f)\n", vec4.x, vec4.y, vec4.z, vec4.w);
}
static inline void print_float3(simd_float3 vec3)
{
    printf("(%f, %f, %f)\n", vec3.x, vec3.y, vec3.z);
}
static inline void print_float4x4(simd_float4x4 mat)
{
    printf("-------------------------------------------------\n");
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (i == 0)
                printf("|");
            printf("%10f %s", mat.columns[i][j], (i == 3) ? "|\n" : "|");
        }
    }
    printf("-------------------------------------------------\n");
}
