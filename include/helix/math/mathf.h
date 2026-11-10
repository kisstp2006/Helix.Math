#pragma once

#include <helix/core/base/types.h>

#include <cmath>
#include <limits>

namespace helix::mathf
{
    inline constexpr f32 PI = 3.14159274f;
    inline constexpr f32 Deg2Rad = PI * 2.0f / 360.0f;
    inline constexpr f32 Rad2Deg = 1.0f / Deg2Rad;
    inline constexpr f32 Epsilon = std::numeric_limits<f32>::denorm_min();
    inline constexpr f32 Infinity = std::numeric_limits<f32>::infinity();
    inline constexpr f32 NegativeInfinity = -std::numeric_limits<f32>::infinity();

    inline f32 Abs(f32 v) { return std::fabs(v); }
    inline f32 Sqrt(f32 v) { return std::sqrt(v); }
    inline f32 Sin(f32 v) { return std::sin(v); }
    inline f32 Cos(f32 v) { return std::cos(v); }
    inline f32 Tan(f32 v) { return std::tan(v); }
    inline f32 Asin(f32 v) { return std::asin(v); }
    inline f32 Acos(f32 v) { return std::acos(v); }
    inline f32 Atan(f32 v) { return std::atan(v); }
    inline f32 Atan2(f32 y, f32 x) { return std::atan2(y, x); }
    inline f32 Pow(f32 v, f32 p) { return std::pow(v, p); }
    inline f32 Exp(f32 v) { return std::exp(v); }
    inline f32 Log(f32 v) { return std::log(v); }
    inline f32 Log(f32 v, f32 base) { return std::log(v) / std::log(base); }
    inline f32 Log10(f32 v) { return std::log10(v); }
    inline f32 Floor(f32 v) { return std::floor(v); }
    inline f32 Ceil(f32 v) { return std::ceil(v); }
    inline f32 Round(f32 v) { return std::nearbyint(v); }
    inline i32 FloorToInt(f32 v) { return i32(std::floor(v)); }
    inline i32 CeilToInt(f32 v) { return i32(std::ceil(v)); }
    inline i32 RoundToInt(f32 v) { return i32(std::nearbyint(v)); }
    inline f32 Sign(f32 v) { return v >= 0.0f ? 1.0f : -1.0f; }
    inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
    inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }
    inline i32 Min(i32 a, i32 b) { return a < b ? a : b; }
    inline i32 Max(i32 a, i32 b) { return a > b ? a : b; }

    inline f32 Clamp(f32 v, f32 lo, f32 hi) { return v < lo ? lo : (v > hi ? hi : v); }
    inline i32 Clamp(i32 v, i32 lo, i32 hi) { return v < lo ? lo : (v > hi ? hi : v); }
    inline f32 Clamp01(f32 v) { return Clamp(v, 0.0f, 1.0f); }

    inline f32 Lerp(f32 a, f32 b, f32 t) { return a + (b - a) * Clamp01(t); }
    inline f32 LerpUnclamped(f32 a, f32 b, f32 t) { return a + (b - a) * t; }
    inline f32 InverseLerp(f32 a, f32 b, f32 v) { return a != b ? Clamp01((v - a) / (b - a)) : 0.0f; }

    inline bool Approximately(f32 a, f32 b)
    {
        return Abs(b - a) < Max(1e-6f * Max(Abs(a), Abs(b)), Epsilon * 8.0f);
    }

    inline f32 Repeat(f32 t, f32 length) { return Clamp(t - Floor(t / length) * length, 0.0f, length); }

    inline f32 PingPong(f32 t, f32 length)
    {
        t = Repeat(t, length * 2.0f);
        return length - Abs(t - length);
    }

    inline f32 DeltaAngle(f32 current, f32 target)
    {
        f32 delta = Repeat(target - current, 360.0f);
        return delta > 180.0f ? delta - 360.0f : delta;
    }

    inline f32 LerpAngle(f32 a, f32 b, f32 t)
    {
        f32 delta = Repeat(b - a, 360.0f);
        if (delta > 180.0f)
            delta -= 360.0f;
        return a + delta * Clamp01(t);
    }

    inline f32 MoveTowards(f32 current, f32 target, f32 maxDelta)
    {
        if (Abs(target - current) <= maxDelta)
            return target;
        return current + Sign(target - current) * maxDelta;
    }

    inline f32 MoveTowardsAngle(f32 current, f32 target, f32 maxDelta)
    {
        f32 delta = DeltaAngle(current, target);
        if (-maxDelta < delta && delta < maxDelta)
            return target;
        return MoveTowards(current, current + delta, maxDelta);
    }

    inline f32 SmoothStep(f32 from, f32 to, f32 t)
    {
        t = Clamp01(t);
        t = -2.0f * t * t * t + 3.0f * t * t;
        return to * t + from * (1.0f - t);
    }

    f32 SmoothDamp(f32 current, f32 target, f32& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime);
    f32 SmoothDampAngle(f32 current, f32 target, f32& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime);

    inline bool IsPowerOfTwo(i32 v) { return v > 0 && (v & (v - 1)) == 0; }
    i32 NextPowerOfTwo(i32 v);
    i32 ClosestPowerOfTwo(i32 v);

    f32 GammaToLinearSpace(f32 v);
    f32 LinearToGammaSpace(f32 v);
    f32 PerlinNoise(f32 x, f32 y);
}
