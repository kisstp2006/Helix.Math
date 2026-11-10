#pragma once

#include <helix/math/vector.h>

namespace helix
{
    struct Color32;

    struct Color
    {
        f32 r = 0, g = 0, b = 0, a = 0;

        constexpr Color() = default;
        constexpr Color(f32 r0, f32 g0, f32 b0, f32 a0 = 1.0f) : r(r0), g(g0), b(b0), a(a0) {}

        static constexpr Color red() { return {1, 0, 0, 1}; }
        static constexpr Color green() { return {0, 1, 0, 1}; }
        static constexpr Color blue() { return {0, 0, 1, 1}; }
        static constexpr Color white() { return {1, 1, 1, 1}; }
        static constexpr Color black() { return {0, 0, 0, 1}; }
        static constexpr Color yellow() { return {1, 0.921568632f, 0.0156862754f, 1}; }
        static constexpr Color cyan() { return {0, 1, 1, 1}; }
        static constexpr Color magenta() { return {1, 0, 1, 1}; }
        static constexpr Color gray() { return {0.5f, 0.5f, 0.5f, 1}; }
        static constexpr Color clear() { return {0, 0, 0, 0}; }

        f32& operator[](int i) { return (&r)[i]; }
        f32 operator[](int i) const { return (&r)[i]; }

        Color linear() const;
        Color gamma() const;
        f32 grayscale() const { return 0.299f * r + 0.587f * g + 0.114f * b; }
        f32 maxColorComponent() const { return mathf::Max(mathf::Max(r, g), b); }

        static Color Lerp(Color a, Color b, f32 t);
        static constexpr Color LerpUnclamped(Color x, Color y, f32 t)
        {
            return {x.r + (y.r - x.r) * t, x.g + (y.g - x.g) * t, x.b + (y.b - x.b) * t, x.a + (y.a - x.a) * t};
        }
        static void RGBToHSV(Color rgb, f32& h, f32& s, f32& v);
        static Color HSVToRGB(f32 h, f32 s, f32 v, bool hdr = true);

        constexpr Color operator+(Color o) const { return {r + o.r, g + o.g, b + o.b, a + o.a}; }
        constexpr Color operator-(Color o) const { return {r - o.r, g - o.g, b - o.b, a - o.a}; }
        constexpr Color operator*(Color o) const { return {r * o.r, g * o.g, b * o.b, a * o.a}; }
        constexpr Color operator*(f32 s) const { return {r * s, g * s, b * s, a * s}; }
        constexpr Color operator/(f32 s) const { return {r / s, g / s, b / s, a / s}; }
        constexpr bool operator==(Color o) const { return Vector4(r, g, b, a) == Vector4(o.r, o.g, o.b, o.a); }
        operator Color32() const;
    };

    struct Color32
    {
        u8 r = 0, g = 0, b = 0, a = 0;

        constexpr Color32() = default;
        constexpr Color32(u8 r0, u8 g0, u8 b0, u8 a0) : r(r0), g(g0), b(b0), a(a0) {}
        constexpr operator Color() const { return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}; }
        constexpr bool operator==(const Color32&) const = default;
    };
}
