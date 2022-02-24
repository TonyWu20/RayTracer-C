#pragma once
#include <assert.h>
#include <types/types.h>

/* @abstract: Returns a scaling matrix by (x, y , z) */
static inline FUNC Matrix_4x4 scaling_matrix(simd_float3 xyz);
/* @abstract: Returns a scaling matrix by (x, y , z) */
static inline FUNC Matrix_4x4 scaling_matrix(float x, float y, float z);
/* @abstract: Returns a translation matrix by (tx, ty, tz) */
static inline FUNC Matrix_4x4 translation_matrix(VectorXYZ xyz);
/* @abstract: Returns a translation matrix by (tx, ty, tz) */
static inline FUNC Matrix_4x4 translation_matrix(float x, float y, float z);
/* @abstract: Returns a shearing matrix */
static inline Matrix_4x4 shearing_matrix(float xy, float xz, float yx, float yz,
                                         float zx, float zy);
/* @abstract: Rotation matrix from quaternion */
static inline Matrix_4x4 rotation_matrix(float angle, VectorXYZ axis);
/* @abstract: Returns a view transformation matrix */
/* @params: simd_float4 *point(xyz, 1), simd_float4 *to(xyz, 1), simd_float4
 * *up(xyz, 0) */
static inline Matrix_4x4 view_transform(Point *from, Point *to,
                                        Vector *upVector);

#pragma mark -Implementation
static inline FUNC Matrix_4x4 scaling_matrix(simd_float3 xyz)
{
    Matrix_4x4 matrix = simd_diagonal_matrix(simd_make_float4(xyz, 1));
    return matrix;
}
static inline FUNC Matrix_4x4 scaling_matrix(float x, float y, float z)
{
    Matrix_4x4 matrix = simd_diagonal_matrix(simd_make_float4(x, y, z, 1));
    return matrix;
}

static inline FUNC Matrix_4x4 translation_matrix(simd_float3 xyz)
{
    Matrix_4x4 matrix = matrix_identity_float4x4;
    matrix.columns[3] = simd_make_float4(xyz, 1);
    return matrix;
}
static inline FUNC Matrix_4x4 translation_matrix(float x, float y, float z)
{
    Matrix_4x4 matrix = matrix_identity_float4x4;
    matrix.columns[3] = simd_make_float4(x, y, z, 1);
    return matrix;
}

static inline Matrix_4x4 shearing_matrix(float xy, float xz, float yx, float yz,
                                         float zx, float zy)
{
    Matrix_4x4 matrix;
    matrix.columns[0] = (simd_float4){1, yx, zx, 0};
    matrix.columns[1] = (simd_float4){xy, 1, zy, 0};
    matrix.columns[2] = (simd_float4){xz, yz, 1, 0};
    matrix.columns[3] = (simd_float4){0, 0, 0, 1};
    return matrix;
}
static inline Matrix_4x4 rotation_matrix(float angle, VectorXYZ axis)
{
    return simd_matrix4x4(simd_quaternion(angle, axis));
}
static inline Matrix_4x4 view_transform(Point *from, Point *to,
                                        Vector *upVector)
{
    assert(from->w == 1.0);
    assert(to->w == 1.0);
    assert(upVector->w == 0);
    Vector forward = simd_normalize(*to - *from);
    VectorXYZ right = simd_cross(simd_make_float3(forward),
                                 simd_make_float3(simd_normalize(*upVector)));
    VectorXYZ trueUpVector = simd_cross(simd_make_float3(forward), right);

    Matrix_4x4 orientation = {
        (simd_float4){right.x, trueUpVector.x, -forward.x, 0},
        (simd_float4){right.y, trueUpVector.y, -forward.y, 0},
        (simd_float4){right.z, trueUpVector.z, -forward.z, 0},
        (simd_float4){0, 0, 0, 1},
    };
    Matrix_4x4 translation = translation_matrix(simd_make_float3(-1 * (*from)));
    return simd_mul(orientation, translation);
}
