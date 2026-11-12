#include <Helix/Math/Random.h>

#include <Helix/Core/Types/Guid.h>

namespace helix
{
    using namespace mathf;

    void RandomGenerator::InitState(u32 seed)
    {
        state_.s0 = seed;
        state_.s1 = state_.s0 * 1812433253u + 1u;
        state_.s2 = state_.s1 * 1812433253u + 1u;
        state_.s3 = state_.s2 * 1812433253u + 1u;
    }

    u32 RandomGenerator::NextU32()
    {
        u32 t = state_.s0 ^ (state_.s0 << 11);
        state_.s0 = state_.s1;
        state_.s1 = state_.s2;
        state_.s2 = state_.s3;
        state_.s3 = state_.s3 ^ (state_.s3 >> 19) ^ t ^ (t >> 8);
        return state_.s3;
    }

    f32 RandomGenerator::value() { return f32(NextU32() & 0x7FFFFFu) * (1.0f / 8388607.0f); }

    f32 RandomGenerator::Range(f32 min, f32 max) { return min + (max - min) * value(); }

    i32 RandomGenerator::Range(i32 min, i32 max)
    {
        if (min == max)
            return min;
        if (min < max)
            return i32(i64(min) + i64(NextU32() % u32(i64(max) - i64(min))));
        return i32(i64(min) - i64(NextU32() % u32(i64(min) - i64(max))));
    }

    Vector3 RandomGenerator::onUnitSphere()
    {
        f32 z = Range(-1.0f, 1.0f);
        f32 a = Range(0.0f, 2.0f * PI);
        f32 r = mathf::Sqrt(mathf::Max(0.0f, 1.0f - z * z));
        return {r * Cos(a), r * Sin(a), z};
    }

    Vector3 RandomGenerator::insideUnitSphere() { return onUnitSphere() * Pow(value(), 1.0f / 3.0f); }

    Vector2 RandomGenerator::insideUnitCircle()
    {
        f32 a = Range(0.0f, 2.0f * PI);
        f32 r = mathf::Sqrt(value());
        return {r * Cos(a), r * Sin(a)};
    }

    Quaternion RandomGenerator::rotation()
    {
        return Quaternion::Normalize({Range(-1.0f, 1.0f), Range(-1.0f, 1.0f), Range(-1.0f, 1.0f), Range(-1.0f, 1.0f)});
    }

    Quaternion RandomGenerator::rotationUniform()
    {
        f32 u1 = value(), u2 = value() * 2.0f * PI, u3 = value() * 2.0f * PI;
        f32 a = mathf::Sqrt(1.0f - u1), b = mathf::Sqrt(u1);
        return {a * Sin(u2), a * Cos(u2), b * Sin(u3), b * Cos(u3)};
    }

    Vector2 RandomGenerator::onUnitCircle()
    {
        f32 a = Range(0.0f, 2.0f * PI);
        return {Cos(a), Sin(a)};
    }

    Color RandomGenerator::ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax, f32 valueMin, f32 valueMax, f32 alphaMin,
                                    f32 alphaMax)
    {
        f32 h = mathf::Lerp(hueMin, hueMax, value());
        f32 s = mathf::Lerp(saturationMin, saturationMax, value());
        f32 v = mathf::Lerp(valueMin, valueMax, value());
        Color c = Color::HSVToRGB(h, s, v, true);
        c.a = mathf::Lerp(alphaMin, alphaMax, value());
        return c;
    }

    RandomGenerator& Random::Generator()
    {
        static RandomGenerator generator = [] {
            u32 seed = 0;
            FillRandomBytes(&seed, sizeof(seed));
            return RandomGenerator(seed);
        }();
        return generator;
    }

    void Random::InitState(i32 seed) { Generator().InitState(u32(seed)); }
    Random::State Random::state() { return Generator().GetState(); }
    void Random::SetState(State value) { Generator().SetState(value); }
    f32 Random::value() { return Generator().value(); }
    f32 Random::Range(f32 minInclusive, f32 maxInclusive) { return Generator().Range(minInclusive, maxInclusive); }
    i32 Random::Range(i32 minInclusive, i32 maxExclusive) { return Generator().Range(minInclusive, maxExclusive); }
    Vector3 Random::insideUnitSphere() { return Generator().insideUnitSphere(); }
    Vector3 Random::onUnitSphere() { return Generator().onUnitSphere(); }
    Vector2 Random::insideUnitCircle() { return Generator().insideUnitCircle(); }
    Vector2 Random::onUnitCircle() { return Generator().onUnitCircle(); }
    Quaternion Random::rotation() { return Generator().rotation(); }
    Quaternion Random::rotationUniform() { return Generator().rotationUniform(); }
    Color Random::ColorHSV() { return Generator().ColorHSV(); }
    Color Random::ColorHSV(f32 hueMin, f32 hueMax) { return Generator().ColorHSV(hueMin, hueMax); }
    Color Random::ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax)
    {
        return Generator().ColorHSV(hueMin, hueMax, saturationMin, saturationMax);
    }
    Color Random::ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax, f32 valueMin, f32 valueMax)
    {
        return Generator().ColorHSV(hueMin, hueMax, saturationMin, saturationMax, valueMin, valueMax);
    }
    Color Random::ColorHSV(f32 hueMin, f32 hueMax, f32 saturationMin, f32 saturationMax, f32 valueMin, f32 valueMax, f32 alphaMin, f32 alphaMax)
    {
        return Generator().ColorHSV(hueMin, hueMax, saturationMin, saturationMax, valueMin, valueMax, alphaMin, alphaMax);
    }
}
