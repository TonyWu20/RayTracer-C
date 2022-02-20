#pragma once
#include <math.h>
#define PI (atan(1) * 4)
#define VEC_OVERLOAD __attribute__((__overloadable__))
#define VEC_INLINE __attribute__((__always_inline__))
#define VEC_NODEBUG __attribute__((__nodebug__))
#define VEC_CFUNC VEC_OVERLOAD VEC_INLINE VEC_NODEBUG

/* Definition */
typedef union
{
    float xyzw[4];
    struct
    {
        float x;
        float y;
        float z;
        float w;
    };
} vec_float4;

typedef union
{
    float xyz[3];
    struct
    {
        float x, y, z;
    };
} vec_float3;

typedef union
{
    float xy[2];
    struct
    {
        float x, y;
    };
} vec_float2;

typedef float vec_float;

typedef union
{
    vec_float2 columns[2];
    struct
    {
        vec_float2 i, j;
    };
} matrix_float2x2;

typedef union
{
    vec_float3 columns[3];
    struct
    {
        vec_float3 i, j, k;
    };
} matrix_float3x3;

typedef union
{
    vec_float4 columns[4];
    struct
    {
        vec_float4 i, j, k, t;
    };
} matrix_float4x4;

typedef union
{
    float xyzr[4];
    struct
    {
        float x, y, z, r;
    };
} vec_quatd;

/* @abstract: create a vector from two float values */
static inline VEC_CFUNC vec_float2 vec_make_float2(float x, float y);
/* @abstract: create a vector from an array of float */
static inline VEC_CFUNC vec_float2 vec_make_float2(float *vec2);
/* @abstract: Truncate a vec_float3 to return a vec_float2 */
static inline VEC_CFUNC vec_float2 vec_make_float2(vec_float3 other);
/* @abstract: Truncate a vec_float4 to return a vec_float2 */
static inline VEC_CFUNC vec_float2 vec_make_float2(vec_float4 other);
/* @abstract: Create a vector from three float values */
static inline VEC_CFUNC vec_float3 vec_make_float3(float x, float y, float z);
/* @abstract: Create a vector from an array of three float values */
static inline VEC_CFUNC vec_float3 vec_make_float3(float *vec3);
/* @abstract: Create a vec_float3 from xy of vec_float2 and given z */
static inline VEC_CFUNC vec_float3 vec_make_float3(vec_float2 other, float z);
/* @abstract: Truncate a vec_float4, use its xyz in the new vector*/
static inline VEC_CFUNC vec_float3 vec_make_float3(vec_float4 other);
/* @abstract: Create a vector of 4 float vals */
static inline VEC_CFUNC vec_float4 vec_make_float4(float x, float y, float z,
                                                   float w);
/* @abstract: Create a vector from an array of 4 float vals */
static inline VEC_CFUNC vec_float4 vec_make_float4(float *vec4);
/* @abstract: Create a 4d vector from a 3d vector, and specify the w */
static inline VEC_CFUNC vec_float4 vec_make_float4(vec_float3 other, float w);
/* @abstract: Create a quaternion by giving the angle and rotation axis */
static inline VEC_CFUNC vec_quatd vec_make_quaternion(float angle,
                                                      vec_float3 axis);
/* @abstract: Create a quaternion by specifying the imaginary part and real
 * part*/
static inline VEC_CFUNC vec_quatd vec_make_quaternion(vec_float3 imag,
                                                      float real);
/* @abstract: Create a quaternion from a 4d vector */
static inline VEC_CFUNC vec_quatd vec_make_quaternion(vec_float4 other);

/* @abstract: Dot product of two 2d vectors */
static inline VEC_CFUNC float vec_dot(vec_float2 u, vec_float2 v);
/* @abstract: Dot product of two 3d vectors */
static inline VEC_CFUNC float vec_dot(vec_float3 u, vec_float3 v);
/* @abstract: Euclidean length */
static inline VEC_CFUNC float vec_length(vec_float2 u);
/* @abstract: Euclidean length */
static inline VEC_CFUNC float vec_length(vec_float3 u);
/* @abstract: Euclidean length, ignoring w */
static inline VEC_CFUNC float vec_length(vec_float4 u);
/* @abstract: Returns a normalized 2d vector */
static inline VEC_CFUNC vec_float2 vec_normalize(vec_float2 u);
/* @abstract: Returns a normalized 3d vector */
static inline VEC_CFUNC vec_float3 vec_normalize(vec_float3 u);
/* @abstract: Returns a normalized 4d vector, norm is based on xyz*/
static inline VEC_CFUNC vec_float4 vec_normalize(vec_float4 u);
/* @abstract: Returns the angle between 2d vectors u and v */
static inline VEC_CFUNC float vec_angle_uv(vec_float2 u, vec_float2 v);
/* @abstract: Returns the angle between 3d vectors u and v */
static inline VEC_CFUNC float vec_angle_uv(vec_float3 u, vec_float3 v);
/* @abstract: Returns the cross product of 3d vectors u and v */
static inline VEC_CFUNC vec_float3 vec_cross(vec_float3 u, vec_float3 v);
/* @abstract: Perform the rotation defined by the quaternion q on the
 * 3d vector v */
static inline VEC_CFUNC vec_float3 vec_act(vec_quatd q, vec_float3 v);
/* @abstract: Perform the rotation defined by the quaternion q on the
 * 3x3 matrix */
static inline VEC_CFUNC matrix_float3x3 vec_act(vec_quatd q, matrix_float3x3 m);
/* @abstract: Returns the imaginary part of the quaternion */
static inline VEC_CFUNC vec_float3 vec_imag(vec_quatd q);
/* @abstract: Returns the real part of the quaternion */
static inline VEC_CFUNC float vec_real(vec_quatd q);
/* @abstract: Returns a 4 by 4 matrix describing a translation between
 * the input *3d* vectors */
static inline VEC_CFUNC matrix_float4x4 vec_translate(vec_float3 from,
                                                      vec_float3 to);

/* @abstract: Scale the 2d vector c = a*v; */
static inline VEC_CFUNC vec_float2 vec_mul(float a, vec_float2 v);
/* @abstract: Scale the 3d vector c = a*v; */
static inline VEC_CFUNC vec_float3 vec_mul(float a, vec_float3 v);
/* @abstract: Scale the 4d vector c = a*v; */
static inline VEC_CFUNC vec_float4 vec_mul(float a, vec_float4 v);
/* @abstract: Linear combination of 2d vectors x and y
 * c = a*x + b*y */
static inline VEC_CFUNC vec_float2 vec_linear_comb(float a, vec_float2 x,
                                                   float b, vec_float2 y);
/* @abstract: Linear combination of 3d vectors x and y
 * c = a*x + b*y */
static inline VEC_CFUNC vec_float3 vec_linear_comb(float a, vec_float3 x,
                                                   float b, vec_float3 y);
/* @abstract: Linear combination of 4d vectors x and y
 * c = a*x + b*y */
static inline VEC_CFUNC vec_float4 vec_linear_comb(float a, vec_float4 x,
                                                   float b, vec_float4 y);
/* @abstract: c = x + y by vec_linear_comb (2d vector) */
static inline VEC_CFUNC vec_float2 vec_add(vec_float2 x, vec_float2 y);
/* @abstract: c = x + y by vec_linear_comb (3d vector) */
static inline VEC_CFUNC vec_float3 vec_add(vec_float3 x, vec_float3 y);
/* @abstract: c = x + y by vec_linear_comb (4d vector) */
static inline VEC_CFUNC vec_float4 vec_add(vec_float4 x, vec_float4 y);
/* @abstract: c = x - y by vec_linear_comb (2d vector) */
static inline VEC_CFUNC vec_float2 vec_sub(vec_float2 x, vec_float2 y);
/* @abstract: c = x - y by vec_linear_comb (3d vector) */
static inline VEC_CFUNC vec_float3 vec_sub(vec_float3 x, vec_float3 y);
/* @abstract: c = x - y by vec_linear_comb (4d vector) */
static inline VEC_CFUNC vec_float4 vec_sub(vec_float4 x, vec_float4 y);
/* @abstract: 2 by 2 Matrix and 2d vector multiplication */
static inline VEC_CFUNC vec_float2 vec_mul(matrix_float2x2 mat_2x2,
                                           vec_float2 u2);
/* @abstract: 3 by 3 Matrix and 3d vector multiplication */
static inline VEC_CFUNC vec_float3 vec_mul(matrix_float3x3 mat_3x3,
                                           vec_float3 u3);
/* @abstract: 4 by 4 Matrix and 4d vector multiplication */
static inline VEC_CFUNC vec_float4 vec_mul(matrix_float4x4 mat_4x4,
                                           vec_float4 u4);
/* @abstract: Fractional coordinate matrix */
static inline VEC_CFUNC matrix_float3x3 vec_fracCoordMat(matrix_float3x3 u);

/* Implementation */
static inline VEC_CFUNC vec_float2 vec_make_float2(float x, float y)
{
    vec_float2 result;
    result.x = x;
    result.y = y;
    return result;
}
static inline VEC_CFUNC vec_float2 vec_make_float2(float *vec2)
{
    return vec_make_float2(vec2[0], vec2[1]);
}
static inline VEC_CFUNC vec_float2 vec_make_float2(vec_float3 other)
{
    vec_float2 result;
    result.x = other.x;
    result.y = other.y;
    return result;
}
static inline VEC_CFUNC vec_float2 vec_make_float2(vec_float4 other)
{
    vec_float2 result;
    result.x = other.x;
    result.y = other.y;
    return result;
}
static inline VEC_CFUNC vec_float3 vec_make_float3(float x, float y, float z)
{
    vec_float3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}
static inline VEC_CFUNC vec_float3 vec_make_float3(float *vec3)
{
    return vec_make_float3(vec3[0], vec3[1], vec3[2]);
}
static inline VEC_CFUNC vec_float3 vec_make_float3(vec_float4 other)
{
    vec_float3 result;
    for (int i = 0; i < 3; ++i)
    {
        result.xyz[i] = other.xyzw[i];
    }
    return result;
}
static inline VEC_CFUNC vec_float4 vec_make_float4(float x, float y, float z,
                                                   float w)
{
    vec_float4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}
static inline VEC_CFUNC vec_float4 vec_make_float4(float *vec4)
{
    return vec_make_float4(vec4[0], vec4[1], vec4[2], vec4[3]);
}
static inline VEC_CFUNC vec_float4 vec_make_float4(vec_float3 other, float w)
{
    vec_float4 result;
    for (int i = 0; i < 3; ++i)
    {
        result.xyzw[i] = other.xyz[i];
    }
    result.w = w;
    return result;
}
static inline VEC_CFUNC vec_quatd vec_make_quaternion(vec_float3 imag,
                                                      float real)
{
    vec_quatd result;
    for (int i = 0; i < 3; ++i)
    {
        result.xyzr[i] = imag.xyz[i];
    }
    result.r = real;
    return result;
}
static inline VEC_CFUNC vec_quatd vec_make_quaternion(vec_float4 other)
{
    vec_quatd result;
    for (int i = 0; i < 4; ++i)
    {
        result.xyzr[i] = other.xyzw[i];
    }
    return result;
}
static inline VEC_CFUNC vec_quatd vec_make_quaternion(float angle,
                                                      vec_float3 axis)
{
    vec_float3 imag;
    for (int i = 0; i < 3; ++i)
    {
        imag.xyz[i] = sin(angle / 2) * axis.xyz[i];
    }
    float real = cos(angle / 2);
    vec_quatd result = vec_make_quaternion(imag, real);
    return result;
}
static inline VEC_CFUNC vec_float3 vec_imag(vec_quatd q)
{
    vec_float3 imag;
    for (int i = 0; i < 3; ++i)
    {
        imag.xyz[i] = q.xyzr[i];
    }
    return imag;
}
static inline VEC_CFUNC float vec_real(vec_quatd q)
{
    return q.r;
}
static inline VEC_CFUNC vec_float2 vec_mul(float a, vec_float2 x)
{
    vec_float2 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 2; ++i)
    {
        result.xy[i] = x.xy[i] * a;
    }
    return result;
}
static inline VEC_CFUNC vec_float3 vec_mul(float a, vec_float3 x)
{
    vec_float3 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xyz[i] = x.xyz[i] * a;
    }
    return result;
}
static inline VEC_CFUNC vec_float4 vec_mul(float a, vec_float4 x)
{
    vec_float4 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 4; ++i)
    {
        result.xyzw[i] = x.xyzw[i] * a;
    }
    return result;
}
static inline VEC_CFUNC vec_float2 vec_linear_comb(float a, vec_float2 x,
                                                   float b, vec_float2 y)
{
    vec_float2 result;
    vec_float2 aX = vec_mul(a, x), bY = vec_mul(b, y);
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xy[i] = aX.xy[i] + bY.xy[i];
    }
    return result;
}
static inline VEC_CFUNC vec_float3 vec_linear_comb(float a, vec_float3 x,
                                                   float b, vec_float3 y)
{
    vec_float3 result;
    vec_float3 aX = vec_mul(a, x), bY = vec_mul(b, y);
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xyz[i] = aX.xyz[i] + bY.xyz[i];
    }
    return result;
}
static inline VEC_CFUNC vec_float4 vec_linear_comb(float a, vec_float4 x,
                                                   float b, vec_float4 y)
{
    vec_float4 result;
    vec_float4 aX = vec_mul(a, x), bY = vec_mul(b, y);
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 4; ++i)
    {
        result.xyzw[i] = aX.xyzw[i] + bY.xyzw[i];
    }
    return result;
}
static inline VEC_CFUNC vec_float2 vec_add(vec_float2 x, vec_float2 y)
{
    return vec_linear_comb(1, x, 1, y);
}
static inline VEC_CFUNC vec_float3 vec_add(vec_float3 x, vec_float3 y)
{
    return vec_linear_comb(1, x, 1, y);
}
static inline VEC_CFUNC vec_float4 vec_add(vec_float4 x, vec_float4 y)
{
    return vec_linear_comb(1, x, 1, y);
}
static inline VEC_CFUNC vec_float2 vec_sub(vec_float2 x, vec_float2 y)
{
    return vec_linear_comb(1, x, -1, y);
}
static inline VEC_CFUNC vec_float3 vec_sub(vec_float3 x, vec_float3 y)
{
    return vec_linear_comb(1, x, -1, y);
}
static inline VEC_CFUNC vec_float4 vec_sub(vec_float4 x, vec_float4 y)
{
    return vec_linear_comb(1, x, -1, y);
}
static inline VEC_CFUNC vec_float2 vec_mul(matrix_float2x2 mat_2x2,
                                           vec_float2 u2)
{
    vec_float2 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            result.xy[i] = mat_2x2.columns[j].xy[i] * u2.xy[j];
        }
    }
    return result;
}
static inline VEC_CFUNC vec_float3 vec_mul(matrix_float3x3 mat_3x3,
                                           vec_float3 u3)
{
    vec_float3 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            result.xyz[i] = mat_3x3.columns[j].xyz[i] * u3.xyz[j];
        }
    }
    return result;
}
static inline VEC_CFUNC vec_float4 vec_mul(matrix_float4x4 mat_4x4,
                                           vec_float4 u4)
{
    vec_float4 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            result.xyzw[j] = mat_4x4.columns[i].xyzw[j] * u4.xyzw[i];
        }
    }
    return result;
}
static inline VEC_CFUNC matrix_float4x4 vec_translate(vec_float3 from,
                                                      vec_float3 to)
{
    vec_float3 fromTo = vec_sub(to, from);
    matrix_float4x4 transMat;
    transMat.i = vec_make_float4(1, 0, 0, 0);
    transMat.j = vec_make_float4(0, 1, 0, 0);
    transMat.k = vec_make_float4(0, 0, 1, 0);
    transMat.t = vec_make_float4(fromTo.x, fromTo.y, fromTo.z, 1);
    return transMat;
}

static inline VEC_CFUNC float vec_dot(vec_float2 u, vec_float2 v)
{
    float result = 0.0;
    // clang-format off
    #pragma clang loop vectorize(enable) interleave(enable)
    // clang-format on
    for (int i = 0; i < 2; ++i)
    {
        result += u.xy[i] * v.xy[i];
    }
    return result;
}
static inline VEC_CFUNC float vec_dot(vec_float3 u, vec_float3 v)
{
    float result = 0.0;
    // clang-format off
    #pragma clang loop vectorize(enable) interleave(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result += u.xyz[i] * v.xyz[i];
    }
    return result;
}

static inline VEC_CFUNC float vec_length(vec_float2 u)
{
    float result = 0.0;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 2; ++i)
    {
        result += u.xy[i] * u.xy[i];
    }
    return sqrt(result);
}
static inline VEC_CFUNC float vec_length(vec_float3 u)
{
    float result = 0.0;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result += u.xyz[i] * u.xyz[i];
    }
    return sqrt(result);
}
static inline VEC_CFUNC float vec_length(vec_float4 u)
{
    float result = 0.0;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result += u.xyzw[i] * u.xyzw[i];
    }
    return sqrt(result);
}
static inline VEC_CFUNC vec_float2 vec_normalize(vec_float2 u)
{
    float uLength = vec_length(u);
    vec_float2 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 2; ++i)
    {
        result.xy[i] = u.xy[i] / uLength;
    }
    return result;
}
static inline VEC_CFUNC vec_float3 vec_normalize(vec_float3 u)
{
    float uLength = vec_length(u);
    vec_float3 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xyz[i] = u.xyz[i] / uLength;
    }
    return result;
}
static inline VEC_CFUNC vec_float4 vec_normalize(vec_float4 u)
{
    float uLength = vec_length(u);
    vec_float4 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xyzw[i] = u.xyzw[i] / uLength;
    }
    return result;
}
static inline VEC_CFUNC float vec_angle_uv(vec_float2 u, vec_float2 v)
{
    float dotProduct = vec_dot(u, v);
    float result = acos(dotProduct / (vec_length(u) * vec_length(v)));
    return result;
}
static inline VEC_CFUNC float vec_angle_uv(vec_float3 u, vec_float3 v)
{
    float dotProduct = vec_dot(u, v);
    float result = acos(dotProduct / (vec_length(u) * vec_length(v)));
    return result;
}
static inline VEC_CFUNC vec_float3 vec_cross(vec_float3 u, vec_float3 v)
{
    vec_float3 cross;
    cross.x = u.y * v.z - u.z * v.y; // a2b3 - a3b2
    cross.y = u.z * v.x - u.x * v.z; // a3b1 - a1b3
    cross.z = u.x * v.y - u.y * v.x; // a1b2 - a2b1
    return cross;
}
static inline VEC_CFUNC vec_float3 vec_centroid(vec_float3 *points,
                                                int pointNums)
{
    vec_float3 result = {{0, 0, 0}};
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < pointNums; ++i)
    {
        result.x += points[i].x;
        result.y += points[i].y;
        result.z += points[i].z;
    }
    result.x /= (float)pointNums;
    result.y /= (float)pointNums;
    result.z /= (float)pointNums;
    return result;
}
static inline VEC_CFUNC vec_float3 vec_act(vec_quatd q, vec_float3 v)
{
    vec_float3 cross = vec_cross(vec_imag(q), v);
    vec_float3 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.xyz[i] =
            v.xyz[i] + vec_real(q) * 2 * cross.xyz[i] + cross.xyz[i];
    }
    return result;
}
static inline VEC_CFUNC matrix_float3x3 vec_act(vec_quatd q, matrix_float3x3 m)
{
    matrix_float3x3 result;
// clang-format off
    #pragma clang loop vectorize(enable)
    // clang-format on
    for (int i = 0; i < 3; ++i)
    {
        result.columns[i] = vec_act(q, m.columns[i]);
    }
    return result;
}
/* Matrix *translate_mat_a_to_b(float *center_a, float *center_b); */

/* Round up number to the bigger nearest tenth.

 * E.g.: 374 -> 380; 376 -> 380;

 * Args: number, float type

 * Returns: rounded float

 */
static inline VEC_CFUNC matrix_float3x3 vec_fracCoordMat(matrix_float3x3 u)
{
    vec_float3 a, b, c;
    a = u.i;
    b = u.j;
    c = u.k;
    float aLen = vec_length(a);
    float bLen = vec_length(b);
    float cLen = vec_length(c);
    float alpha = vec_angle_uv(b, c);
    float beta = vec_angle_uv(a, c);
    float gamma = vec_angle_uv(a, b);
    float vol = vec_dot(a, vec_cross(b, c));
    vec_float3 x_cart = vec_make_float3(1 / aLen, 0, 0);
    vec_float3 y_cart = vec_make_float3(-cos(gamma) / (aLen * sin(gamma)),
                                        1 / (bLen * sin(gamma)), 0);
    vec_float3 z_cart =
        vec_make_float3(bLen * cLen * (cos(alpha) * cos(gamma) - cos(beta)) /
                            (vol * sin(gamma)),
                        aLen * cLen * (cos(beta) * cos(gamma) - cos(alpha)) /
                            (vol * sin(gamma)),
                        aLen * bLen * sin(gamma) / vol);
    matrix_float3x3 toFrac;
    toFrac.i = x_cart;
    toFrac.j = y_cart;
    toFrac.k = z_cart;
    return toFrac;
}
