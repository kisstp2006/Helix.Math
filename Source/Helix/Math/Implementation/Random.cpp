#include <Helix/Math/Random.h>

namespace helix
{
    using namespace mathf;

    void Random::InitState(u32 seed)
    {
        state_.s0 = seed;
        state_.s1 = state_.s0 * 1812433253u + 1u;
        state_.s2 = state_.s1 * 1812433253u + 1u;
        state_.s3 = state_.s2 * 1812433253u + 1u;
    }

    u32 Random::NextU32()
    {
        u32 t = state_.s0 ^ (state_.s0 << 11);
        state_.s0 = state_.s1;
        state_.s1 = state_.s2;
        state_.s2 = state_.s3;
        state_.s3 = state_.s3 ^ (state_.s3 >> 19) ^ t ^ (t >> 8);
        return state_.s3;
    }

    f32 Random::value() { return f32(NextU32() & 0x7FFFFFu) * (1.0f / 8388607.0f); }

    f32 Random::Range(f32 min, f32 max) { return min + (max - min) * value(); }

    i32 Random::Range(i32 min, i32 max)
    {
        if (min == max)
            return min;
        if (min < max)
            return i32(i64(min) + i64(NextU32() % u32(i64(max) - i64(min))));
        return i32(i64(min) - i64(NextU32() % u32(i64(min) - i64(max))));
    }

    Vector3 Random::onUnitSphere()
    {
        f32 z = Range(-1.0f, 1.0f);
        f32 a = Range(0.0f, 2.0f * PI);
        f32 r = mathf::Sqrt(mathf::Max(0.0f, 1.0f - z * z));
        return {r * Cos(a), r * Sin(a), z};
    }

    Vector3 Random::insideUnitSphere() { return onUnitSphere() * Pow(value(), 1.0f / 3.0f); }

    Vector2 Random::insideUnitCircle()
    {
        f32 a = Range(0.0f, 2.0f * PI);
        f32 r = mathf::Sqrt(value());
        return {r * Cos(a), r * Sin(a)};
    }

    Quaternion Random::rotation()
    {
        return Quaternion::Normalize({Range(-1.0f, 1.0f), Range(-1.0f, 1.0f), Range(-1.0f, 1.0f), Range(-1.0f, 1.0f)});
    }

    Quaternion Random::rotationUniform()
    {
        f32 u1 = value(), u2 = value() * 2.0f * PI, u3 = value() * 2.0f * PI;
        f32 a = mathf::Sqrt(1.0f - u1), b = mathf::Sqrt(u1);
        return {a * Sin(u2), a * Cos(u2), b * Sin(u3), b * Cos(u3)};
    }
}
