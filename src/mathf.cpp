#include <helix/math/mathf.h>

namespace helix::mathf
{
    f32 SmoothDamp(f32 current, f32 target, f32& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime)
    {
        smoothTime = Max(0.0001f, smoothTime);
        f32 omega = 2.0f / smoothTime;
        f32 x = omega * deltaTime;
        f32 decay = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        f32 change = current - target;
        f32 originalTarget = target;
        f32 maxChange = maxSpeed * smoothTime;
        change = Clamp(change, -maxChange, maxChange);
        target = current - change;
        f32 temp = (velocity + omega * change) * deltaTime;
        velocity = (velocity - omega * temp) * decay;
        f32 output = target + (change + temp) * decay;
        if ((originalTarget - current > 0.0f) == (output > originalTarget))
        {
            output = originalTarget;
            velocity = (output - originalTarget) / deltaTime;
        }
        return output;
    }

    f32 SmoothDampAngle(f32 current, f32 target, f32& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime)
    {
        target = current + DeltaAngle(current, target);
        return SmoothDamp(current, target, velocity, smoothTime, maxSpeed, deltaTime);
    }

    i32 NextPowerOfTwo(i32 v)
    {
        if (v <= 0)
            return 0;
        u32 u = u32(v) - 1;
        u |= u >> 1;
        u |= u >> 2;
        u |= u >> 4;
        u |= u >> 8;
        u |= u >> 16;
        return i32(u + 1);
    }

    i32 ClosestPowerOfTwo(i32 v)
    {
        if (v <= 0)
            return 0;
        i32 next = NextPowerOfTwo(v);
        i32 prev = next >> 1;
        return v - prev < next - v ? prev : next;
    }

    f32 GammaToLinearSpace(f32 v)
    {
        if (v <= 0.04045f)
            return v / 12.92f;
        if (v < 1.0f)
            return Pow((v + 0.055f) / 1.055f, 2.4f);
        return Pow(v, 2.2f);
    }

    f32 LinearToGammaSpace(f32 v)
    {
        if (v <= 0.0f)
            return 0.0f;
        if (v <= 0.0031308f)
            return 12.92f * v;
        if (v < 1.0f)
            return 1.055f * Pow(v, 0.4166667f) - 0.055f;
        return Pow(v, 0.45454545f);
    }

    namespace
    {
        constexpr u8 kPermutation[256] = {
            151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240,
            21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
            237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111,
            229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
            209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217,
            226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,
            223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
            108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241,
            81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
            150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180};

        inline int Perm(int i) { return kPermutation[i & 255]; }
        inline f32 Fade(f32 t) { return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); }

        inline f32 Grad(int hash, f32 x, f32 y)
        {
            switch (hash & 7)
            {
            case 0: return x + y;
            case 1: return -x + y;
            case 2: return x - y;
            case 3: return -x - y;
            case 4: return x;
            case 5: return -x;
            case 6: return y;
            default: return -y;
            }
        }
    }

    // Improved gradient noise remapped to roughly [0, 1], the range the script API documents.
    f32 PerlinNoise(f32 x, f32 y)
    {
        int xi = int(Floor(x)), yi = int(Floor(y));
        f32 xf = x - Floor(x), yf = y - Floor(y);
        f32 u = Fade(xf), v = Fade(yf);
        int aa = Perm(Perm(xi) + yi), ab = Perm(Perm(xi) + yi + 1);
        int ba = Perm(Perm(xi + 1) + yi), bb = Perm(Perm(xi + 1) + yi + 1);
        f32 x1 = LerpUnclamped(Grad(aa, xf, yf), Grad(ba, xf - 1.0f, yf), u);
        f32 x2 = LerpUnclamped(Grad(ab, xf, yf - 1.0f), Grad(bb, xf - 1.0f, yf - 1.0f), u);
        return Clamp01((LerpUnclamped(x1, x2, v) + 1.0f) * 0.5f);
    }
}
