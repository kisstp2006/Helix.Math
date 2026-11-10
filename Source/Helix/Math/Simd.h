#pragma once

#include <Helix/Core/Basics/Platform.h>
#include <Helix/Core/Basics/Types.h>

#if defined(__wasm_simd128__)
#include <wasm_simd128.h>
#define HX_SIMD_WASM 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#include <arm_neon.h>
#define HX_SIMD_NEON 1
#elif defined(__x86_64__) || defined(_M_X64)
#include <emmintrin.h>
#define HX_SIMD_SSE 1
#else
#define HX_SIMD_SCALAR 1
#endif

namespace helix::simd
{
#if HX_SIMD_SSE
    using float4 = __m128;
    HX_FORCE_INLINE float4 Load(const f32* p) { return _mm_loadu_ps(p); }
    HX_FORCE_INLINE void Store(f32* p, float4 v) { _mm_storeu_ps(p, v); }
    HX_FORCE_INLINE float4 Set(f32 x, f32 y, f32 z, f32 w) { return _mm_setr_ps(x, y, z, w); }
    HX_FORCE_INLINE float4 Splat(f32 v) { return _mm_set1_ps(v); }
    HX_FORCE_INLINE float4 Add(float4 a, float4 b) { return _mm_add_ps(a, b); }
    HX_FORCE_INLINE float4 Sub(float4 a, float4 b) { return _mm_sub_ps(a, b); }
    HX_FORCE_INLINE float4 Mul(float4 a, float4 b) { return _mm_mul_ps(a, b); }
    HX_FORCE_INLINE float4 Div(float4 a, float4 b) { return _mm_div_ps(a, b); }
    HX_FORCE_INLINE float4 MulAdd(float4 a, float4 b, float4 c) { return _mm_add_ps(_mm_mul_ps(a, b), c); }
    HX_FORCE_INLINE float4 Min(float4 a, float4 b) { return _mm_min_ps(a, b); }
    HX_FORCE_INLINE float4 Max(float4 a, float4 b) { return _mm_max_ps(a, b); }
    HX_FORCE_INLINE float4 Abs(float4 a) { return _mm_and_ps(a, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff))); }
    HX_FORCE_INLINE float4 Sqrt(float4 a) { return _mm_sqrt_ps(a); }
    HX_FORCE_INLINE float4 Less(float4 a, float4 b) { return _mm_cmplt_ps(a, b); }
    HX_FORCE_INLINE float4 Or(float4 a, float4 b) { return _mm_or_ps(a, b); }
    HX_FORCE_INLINE int MoveMask(float4 a) { return _mm_movemask_ps(a); }
#elif HX_SIMD_NEON
    using float4 = float32x4_t;
    HX_FORCE_INLINE float4 Load(const f32* p) { return vld1q_f32(p); }
    HX_FORCE_INLINE void Store(f32* p, float4 v) { vst1q_f32(p, v); }
    HX_FORCE_INLINE float4 Set(f32 x, f32 y, f32 z, f32 w)
    {
        f32 v[4] = {x, y, z, w};
        return vld1q_f32(v);
    }
    HX_FORCE_INLINE float4 Splat(f32 v) { return vdupq_n_f32(v); }
    HX_FORCE_INLINE float4 Add(float4 a, float4 b) { return vaddq_f32(a, b); }
    HX_FORCE_INLINE float4 Sub(float4 a, float4 b) { return vsubq_f32(a, b); }
    HX_FORCE_INLINE float4 Mul(float4 a, float4 b) { return vmulq_f32(a, b); }
    HX_FORCE_INLINE float4 Div(float4 a, float4 b) { return vdivq_f32(a, b); }
    HX_FORCE_INLINE float4 MulAdd(float4 a, float4 b, float4 c) { return vfmaq_f32(c, a, b); }
    HX_FORCE_INLINE float4 Min(float4 a, float4 b) { return vminq_f32(a, b); }
    HX_FORCE_INLINE float4 Max(float4 a, float4 b) { return vmaxq_f32(a, b); }
    HX_FORCE_INLINE float4 Abs(float4 a) { return vabsq_f32(a); }
    HX_FORCE_INLINE float4 Sqrt(float4 a) { return vsqrtq_f32(a); }
    HX_FORCE_INLINE float4 Less(float4 a, float4 b) { return vreinterpretq_f32_u32(vcltq_f32(a, b)); }
    HX_FORCE_INLINE float4 Or(float4 a, float4 b) { return vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b))); }
    HX_FORCE_INLINE int MoveMask(float4 a)
    {
        uint32x4_t bits = vshrq_n_u32(vreinterpretq_u32_f32(a), 31);
        return int(vgetq_lane_u32(bits, 0) | (vgetq_lane_u32(bits, 1) << 1) | (vgetq_lane_u32(bits, 2) << 2) | (vgetq_lane_u32(bits, 3) << 3));
    }
#elif HX_SIMD_WASM
    using float4 = v128_t;
    HX_FORCE_INLINE float4 Load(const f32* p) { return wasm_v128_load(p); }
    HX_FORCE_INLINE void Store(f32* p, float4 v) { wasm_v128_store(p, v); }
    HX_FORCE_INLINE float4 Set(f32 x, f32 y, f32 z, f32 w) { return wasm_f32x4_make(x, y, z, w); }
    HX_FORCE_INLINE float4 Splat(f32 v) { return wasm_f32x4_splat(v); }
    HX_FORCE_INLINE float4 Add(float4 a, float4 b) { return wasm_f32x4_add(a, b); }
    HX_FORCE_INLINE float4 Sub(float4 a, float4 b) { return wasm_f32x4_sub(a, b); }
    HX_FORCE_INLINE float4 Mul(float4 a, float4 b) { return wasm_f32x4_mul(a, b); }
    HX_FORCE_INLINE float4 Div(float4 a, float4 b) { return wasm_f32x4_div(a, b); }
    HX_FORCE_INLINE float4 MulAdd(float4 a, float4 b, float4 c) { return wasm_f32x4_add(wasm_f32x4_mul(a, b), c); }
    HX_FORCE_INLINE float4 Min(float4 a, float4 b) { return wasm_f32x4_min(a, b); }
    HX_FORCE_INLINE float4 Max(float4 a, float4 b) { return wasm_f32x4_max(a, b); }
    HX_FORCE_INLINE float4 Abs(float4 a) { return wasm_f32x4_abs(a); }
    HX_FORCE_INLINE float4 Sqrt(float4 a) { return wasm_f32x4_sqrt(a); }
    HX_FORCE_INLINE float4 Less(float4 a, float4 b) { return wasm_f32x4_lt(a, b); }
    HX_FORCE_INLINE float4 Or(float4 a, float4 b) { return wasm_v128_or(a, b); }
    HX_FORCE_INLINE int MoveMask(float4 a) { return int(wasm_i32x4_bitmask(a)); }
#else
    struct float4
    {
        f32 v[4];
    };
    inline float4 Load(const f32* p) { return {{p[0], p[1], p[2], p[3]}}; }
    inline void Store(f32* p, float4 a)
    {
        for (int i = 0; i < 4; i++)
            p[i] = a.v[i];
    }
    inline float4 Set(f32 x, f32 y, f32 z, f32 w) { return {{x, y, z, w}}; }
    inline float4 Splat(f32 s) { return {{s, s, s, s}}; }
#define HX_SIMD_SCALAR_OP(name, expr)                \
    inline float4 name(float4 a, float4 b)           \
    {                                                \
        float4 r;                                    \
        for (int i = 0; i < 4; i++)                  \
            r.v[i] = expr;                           \
        return r;                                    \
    }
    HX_SIMD_SCALAR_OP(Add, a.v[i] + b.v[i])
    HX_SIMD_SCALAR_OP(Sub, a.v[i] - b.v[i])
    HX_SIMD_SCALAR_OP(Mul, a.v[i] * b.v[i])
    HX_SIMD_SCALAR_OP(Div, a.v[i] / b.v[i])
    HX_SIMD_SCALAR_OP(Min, a.v[i] < b.v[i] ? a.v[i] : b.v[i])
    HX_SIMD_SCALAR_OP(Max, a.v[i] > b.v[i] ? a.v[i] : b.v[i])
#undef HX_SIMD_SCALAR_OP
    inline float4 MulAdd(float4 a, float4 b, float4 c) { return Add(Mul(a, b), c); }
#endif
}
