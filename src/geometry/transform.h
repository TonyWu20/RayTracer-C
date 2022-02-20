#pragma once
#include <simd/simd.h>

static inline simd_float4x4 scaling_matrix(simd_float3 xyz)
{
    simd_float4x4 matrix = simd_diagonal_matrix(simd_make_float4(xyz, 1));
    return matrix;
}

static inline simd_float4x4 translation_matrix(simd_float3 xyz)
{
    simd_float4x4 matrix = matrix_identity_float4x4;
    matrix.columns[3] = simd_make_float4(xyz, 1);
    return matrix;
}

static inline simd_float4x4 shearing_matrix(double xy, double xz, double yx,
                                            double yz, double zx, double zy)
{
    simd_float4x4 matrix;
    matrix.columns[0] = (simd_float4){1, yx, zx, 0};
    matrix.columns[1] = (simd_float4){xy, 1, zy, 0};
    matrix.columns[2] = (simd_float4){xz, yz, 1, 0};
    matrix.columns[3] = (simd_float4){0, 0, 0, 1};
    return matrix;
}
