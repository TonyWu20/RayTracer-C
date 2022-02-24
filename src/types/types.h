/*
 * This header file typedef some commonly use data type for coding
 * conveniece
 */
#pragma once
#include <simd/simd.h>
#define OVERLOAD __attribute__((__overloadable__))
#define FUNC OVERLOAD

typedef simd_float3 Color;
typedef simd_float4 Point;
typedef simd_float4 Vector;
typedef simd_float4x4 Matrix_4x4;
typedef simd_float3 PointXYZ;
typedef simd_float3 VectorXYZ;

/* @abstract: Wrapper for making a simd_float4 variable representing
 * a point in 3D space
 */
static inline FUNC Point make_Point(float x, float y, float z);
static inline FUNC Point make_Point(PointXYZ xyz);
static inline FUNC Vector make_Vector(float x, float y, float z);
static inline FUNC Vector make_Vector(VectorXYZ);

#pragma mark -Implementations
// Make Point
static inline FUNC Point make_Point(float x, float y, float z)
{
    return simd_make_float4(x, y, z, 1);
}
static inline FUNC Point make_Point(PointXYZ xyz)
{
    return simd_make_float4(xyz, 1);
}
// Make Point Ends

// Make Vector
static inline FUNC Vector make_Vector(float x, float y, float z)
{
    return simd_make_float4(x, y, z, 0);
}
static inline FUNC Vector make_Vector(VectorXYZ xyz)
{
    return simd_make_float4(xyz, 0);
}
// Make Vector Ends
