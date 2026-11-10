#pragma once

#include <Helix/Math/Mathf.h>

namespace helix
{
    struct Vector2
    {
        f32 x = 0, y = 0;

        static constexpr f32 kEpsilon = 1e-5f;
        static constexpr f32 kEpsilonNormalSqrt = 1e-15f;

        constexpr Vector2() = default;
        constexpr Vector2(f32 x0, f32 y0) : x(x0), y(y0) {}

        static constexpr Vector2 zero() { return {0, 0}; }
        static constexpr Vector2 one() { return {1, 1}; }
        static constexpr Vector2 up() { return {0, 1}; }
        static constexpr Vector2 down() { return {0, -1}; }
        static constexpr Vector2 left() { return {-1, 0}; }
        static constexpr Vector2 right() { return {1, 0}; }

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector2 normalized() const;

        static constexpr f32 Dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }
        static f32 Distance(Vector2 a, Vector2 b);
        static Vector2 Lerp(Vector2 a, Vector2 b, f32 t);
        static constexpr Vector2 LerpUnclamped(Vector2 a, Vector2 b, f32 t) { return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t}; }
        static f32 Angle(Vector2 from, Vector2 to);
        static f32 SignedAngle(Vector2 from, Vector2 to);
        static Vector2 MoveTowards(Vector2 current, Vector2 target, f32 maxDistance);
        static Vector2 ClampMagnitude(Vector2 v, f32 maxLength);
        static Vector2 Reflect(Vector2 direction, Vector2 normal);
        static constexpr Vector2 Perpendicular(Vector2 v) { return {-v.y, v.x}; }
        static constexpr Vector2 Scale(Vector2 a, Vector2 b) { return {a.x * b.x, a.y * b.y}; }
        static Vector2 Min(Vector2 a, Vector2 b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y)}; }
        static Vector2 Max(Vector2 a, Vector2 b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y)}; }

        constexpr Vector2 operator-() const { return {-x, -y}; }
        constexpr Vector2 operator+(Vector2 o) const { return {x + o.x, y + o.y}; }
        constexpr Vector2 operator-(Vector2 o) const { return {x - o.x, y - o.y}; }
        constexpr Vector2 operator*(f32 s) const { return {x * s, y * s}; }
        constexpr Vector2 operator*(Vector2 o) const { return {x * o.x, y * o.y}; }
        constexpr Vector2 operator/(f32 s) const { return {x / s, y / s}; }
        constexpr Vector2 operator/(Vector2 o) const { return {x / o.x, y / o.y}; }
        Vector2& operator+=(Vector2 o) { return *this = *this + o; }
        Vector2& operator-=(Vector2 o) { return *this = *this - o; }
        Vector2& operator*=(f32 s) { return *this = *this * s; }
        Vector2& operator/=(f32 s) { return *this = *this / s; }
        constexpr bool operator==(Vector2 o) const { return (*this - o).sqrMagnitude() < kEpsilon * kEpsilon; }
    };

    constexpr Vector2 operator*(f32 s, Vector2 v) { return v * s; }

    struct Vector3
    {
        f32 x = 0, y = 0, z = 0;

        static constexpr f32 kEpsilon = 1e-5f;
        static constexpr f32 kEpsilonNormalSqrt = 1e-15f;

        constexpr Vector3() = default;
        constexpr Vector3(f32 x0, f32 y0, f32 z0) : x(x0), y(y0), z(z0) {}
        constexpr Vector3(f32 x0, f32 y0) : x(x0), y(y0), z(0) {}
        constexpr Vector3(Vector2 v) : x(v.x), y(v.y), z(0) {}
        constexpr explicit operator Vector2() const { return {x, y}; }

        static constexpr Vector3 zero() { return {0, 0, 0}; }
        static constexpr Vector3 one() { return {1, 1, 1}; }
        static constexpr Vector3 up() { return {0, 1, 0}; }
        static constexpr Vector3 down() { return {0, -1, 0}; }
        static constexpr Vector3 left() { return {-1, 0, 0}; }
        static constexpr Vector3 right() { return {1, 0, 0}; }
        static constexpr Vector3 forward() { return {0, 0, 1}; }
        static constexpr Vector3 back() { return {0, 0, -1}; }

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y + z * z; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector3 normalized() const { return Normalize(*this); }

        static constexpr f32 Dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
        static constexpr Vector3 Cross(Vector3 a, Vector3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
        static Vector3 Normalize(Vector3 v);
        static f32 Distance(Vector3 a, Vector3 b);
        static f32 Magnitude(Vector3 v) { return v.magnitude(); }
        static f32 SqrMagnitude(Vector3 v) { return v.sqrMagnitude(); }
        static Vector3 Lerp(Vector3 a, Vector3 b, f32 t);
        static constexpr Vector3 LerpUnclamped(Vector3 a, Vector3 b, f32 t)
        {
            return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t};
        }
        static Vector3 Slerp(Vector3 a, Vector3 b, f32 t);
        static f32 Angle(Vector3 from, Vector3 to);
        static f32 SignedAngle(Vector3 from, Vector3 to, Vector3 axis);
        static Vector3 MoveTowards(Vector3 current, Vector3 target, f32 maxDistance);
        static Vector3 RotateTowards(Vector3 current, Vector3 target, f32 maxRadians, f32 maxMagnitude);
        static Vector3 ClampMagnitude(Vector3 v, f32 maxLength);
        static Vector3 Project(Vector3 v, Vector3 normal);
        static Vector3 ProjectOnPlane(Vector3 v, Vector3 planeNormal);
        static Vector3 Reflect(Vector3 direction, Vector3 normal);
        static void OrthoNormalize(Vector3& normal, Vector3& tangent);
        static Vector3 SmoothDamp(Vector3 current, Vector3 target, Vector3& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime);
        static constexpr Vector3 Scale(Vector3 a, Vector3 b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
        static Vector3 Min(Vector3 a, Vector3 b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y), mathf::Min(a.z, b.z)}; }
        static Vector3 Max(Vector3 a, Vector3 b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y), mathf::Max(a.z, b.z)}; }

        constexpr Vector3 operator-() const { return {-x, -y, -z}; }
        constexpr Vector3 operator+(Vector3 o) const { return {x + o.x, y + o.y, z + o.z}; }
        constexpr Vector3 operator-(Vector3 o) const { return {x - o.x, y - o.y, z - o.z}; }
        constexpr Vector3 operator*(f32 s) const { return {x * s, y * s, z * s}; }
        constexpr Vector3 operator/(f32 s) const { return {x / s, y / s, z / s}; }
        Vector3& operator+=(Vector3 o) { return *this = *this + o; }
        Vector3& operator-=(Vector3 o) { return *this = *this - o; }
        Vector3& operator*=(f32 s) { return *this = *this * s; }
        Vector3& operator/=(f32 s) { return *this = *this / s; }
        constexpr bool operator==(Vector3 o) const { return (*this - o).sqrMagnitude() < kEpsilon * kEpsilon; }
    };

    constexpr Vector3 operator*(f32 s, Vector3 v) { return v * s; }

    struct Vector4
    {
        f32 x = 0, y = 0, z = 0, w = 0;

        static constexpr f32 kEpsilon = 1e-5f;

        constexpr Vector4() = default;
        constexpr Vector4(f32 x0, f32 y0, f32 z0, f32 w0) : x(x0), y(y0), z(z0), w(w0) {}
        constexpr Vector4(f32 x0, f32 y0, f32 z0) : x(x0), y(y0), z(z0), w(0) {}
        constexpr Vector4(Vector3 v) : x(v.x), y(v.y), z(v.z), w(0) {}
        constexpr Vector4(Vector3 v, f32 w0) : x(v.x), y(v.y), z(v.z), w(w0) {}
        constexpr explicit operator Vector3() const { return {x, y, z}; }

        static constexpr Vector4 zero() { return {0, 0, 0, 0}; }
        static constexpr Vector4 one() { return {1, 1, 1, 1}; }

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y + z * z + w * w; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector4 normalized() const;

        static constexpr f32 Dot(Vector4 a, Vector4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
        static Vector4 Lerp(Vector4 a, Vector4 b, f32 t);
        static constexpr Vector4 Scale(Vector4 a, Vector4 b) { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }

        constexpr Vector4 operator-() const { return {-x, -y, -z, -w}; }
        constexpr Vector4 operator+(Vector4 o) const { return {x + o.x, y + o.y, z + o.z, w + o.w}; }
        constexpr Vector4 operator-(Vector4 o) const { return {x - o.x, y - o.y, z - o.z, w - o.w}; }
        constexpr Vector4 operator*(f32 s) const { return {x * s, y * s, z * s, w * s}; }
        constexpr Vector4 operator/(f32 s) const { return {x / s, y / s, z / s, w / s}; }
        constexpr bool operator==(Vector4 o) const { return (*this - o).sqrMagnitude() < kEpsilon * kEpsilon; }
    };

    constexpr Vector4 operator*(f32 s, Vector4 v) { return v * s; }

    struct Vector2Int
    {
        i32 x = 0, y = 0;
        constexpr bool operator==(const Vector2Int&) const = default;
        constexpr Vector2Int operator+(Vector2Int o) const { return {x + o.x, y + o.y}; }
        constexpr Vector2Int operator-(Vector2Int o) const { return {x - o.x, y - o.y}; }
    };

    struct Vector3Int
    {
        i32 x = 0, y = 0, z = 0;
        constexpr bool operator==(const Vector3Int&) const = default;
        constexpr Vector3Int operator+(Vector3Int o) const { return {x + o.x, y + o.y, z + o.z}; }
        constexpr Vector3Int operator-(Vector3Int o) const { return {x - o.x, y - o.y, z - o.z}; }
    };
}
