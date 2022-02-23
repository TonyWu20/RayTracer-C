#pragma once
#include <simd/geometry.h>
#include <simd/simd.h>

/* @abstract: Returns a scaling matrix by (x, y , z) */
static inline simd_float4x4 scaling_matrix(simd_float3 xyz);
/* @abstract: Returns a translation matrix by (tx, ty, tz) */
static inline simd_float4x4 translation_matrix(simd_float3 xyz);
/* @abstract: Returns a shearing matrix */
static inline simd_float4x4 shearing_matrix(double xy, double xz, double yx,
                                            double yz, double zx, double zy);
/* @abstract: Returns a view transformation matrix */
/* @params: simd_float4 *point(xyz, 1), simd_float4 *to(xyz, 1), simd_float4
 * *up(xyz, 0) */
static inline simd_float4x4 view_transform(simd_float4 *from, simd_float4 *to,
                                           simd_float4 *upVector);

#pragma mark -Implementation
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
static inline simd_float4x4 view_transform(simd_float4 *from, simd_float4 *to,
                                           simd_float4 *upVector)
{
    simd_float4 forward = simd_normalize(*to - *from);
    simd_float3 right = simd_cross(simd_make_float3(forward),
                                   simd_make_float3(simd_normalize(*upVector)));
    simd_float3 trueUpVector = simd_cross(simd_make_float3(forward), right);

    simd_float4x4 orientation = {
        (simd_float4){right.x, trueUpVector.x, -forward.x, 0},
        (simd_float4){right.y, trueUpVector.y, -forward.y, 0},
        (simd_float4){right.z, trueUpVector.z, -forward.z, 0},
        (simd_float4){0, 0, 0, 1},
    };
    simd_float4x4 translation =
        translation_matrix(simd_make_float3(-1 * (*from)));
    return simd_mul(orientation, translation);
}
