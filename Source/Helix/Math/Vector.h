#pragma once

#include <Helix/Core/Basics/Tags.h>
#include <Helix/Math/Mathf.h>

// The vector types in the reference's shape: fields, static properties as static constants (Vector3::zero),
// instance properties as member functions (magnitude(), normalized()), the reference's methods and static methods.
// Vectors compare approximately with == (1e-5), exactly with Equals.
namespace helix
{
    struct Vector3;

    HX_STRUCT(Namespace = "HelixEngine")
    struct Vector2
    {
        HX_FIELD() f32 x = 0;
        HX_FIELD() f32 y = 0;

        static constexpr f32 kEpsilon = 1e-5f;
        static constexpr f32 kEpsilonNormalSqrt = 1e-15f;
        static const Vector2 zero, one, up, down, left, right, positiveInfinity, negativeInfinity;

        constexpr Vector2() = default;
        constexpr Vector2(f32 x0, f32 y0) : x(x0), y(y0) {}

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector2 normalized() const { return Normalize(*this); }
        void Normalize() { *this = Normalize(*this); }
        void Set(f32 newX, f32 newY) { x = newX, y = newY; }
        void Scale(Vector2 scale) { x *= scale.x, y *= scale.y; }
        constexpr bool Equals(Vector2 o) const { return x == o.x && y == o.y; }

        static constexpr f32 Dot(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }
        static Vector2 Normalize(Vector2 v);
        static f32 Distance(Vector2 a, Vector2 b);
        static Vector2 Lerp(Vector2 a, Vector2 b, f32 t);
        static constexpr Vector2 LerpUnclamped(Vector2 a, Vector2 b, f32 t) { return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t}; }
        static f32 Angle(Vector2 from, Vector2 to);
        static f32 SignedAngle(Vector2 from, Vector2 to);
        static Vector2 MoveTowards(Vector2 current, Vector2 target, f32 maxDistanceDelta);
        static Vector2 ClampMagnitude(Vector2 vector, f32 maxLength);
        static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal);
        static constexpr Vector2 Perpendicular(Vector2 inDirection) { return {-inDirection.y, inDirection.x}; }
        static constexpr Vector2 Scale(Vector2 a, Vector2 b) { return {a.x * b.x, a.y * b.y}; }
        static Vector2 Min(Vector2 a, Vector2 b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y)}; }
        static Vector2 Max(Vector2 a, Vector2 b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y)}; }
        static Vector2 SmoothDamp(Vector2 current, Vector2 target, Vector2& currentVelocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime);

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

    inline constexpr Vector2 Vector2::zero{0, 0};
    inline constexpr Vector2 Vector2::one{1, 1};
    inline constexpr Vector2 Vector2::up{0, 1};
    inline constexpr Vector2 Vector2::down{0, -1};
    inline constexpr Vector2 Vector2::left{-1, 0};
    inline constexpr Vector2 Vector2::right{1, 0};
    inline constexpr Vector2 Vector2::positiveInfinity{mathf::Infinity, mathf::Infinity};
    inline constexpr Vector2 Vector2::negativeInfinity{mathf::NegativeInfinity, mathf::NegativeInfinity};

    constexpr Vector2 operator*(f32 s, Vector2 v) { return v * s; }

    HX_STRUCT(Namespace = "HelixEngine")
    struct Vector3
    {
        HX_FIELD() f32 x = 0;
        HX_FIELD() f32 y = 0;
        HX_FIELD() f32 z = 0;

        static constexpr f32 kEpsilon = 1e-5f;
        static constexpr f32 kEpsilonNormalSqrt = 1e-15f;
        static const Vector3 zero, one, up, down, left, right, forward, back, positiveInfinity, negativeInfinity;

        constexpr Vector3() = default;
        constexpr Vector3(f32 x0, f32 y0, f32 z0) : x(x0), y(y0), z(z0) {}
        constexpr Vector3(f32 x0, f32 y0) : x(x0), y(y0), z(0) {}
        constexpr Vector3(Vector2 v) : x(v.x), y(v.y), z(0) {}
        constexpr explicit operator Vector2() const { return {x, y}; }

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y + z * z; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector3 normalized() const { return Normalize(*this); }
        void Normalize() { *this = Normalize(*this); }
        void Set(f32 newX, f32 newY, f32 newZ) { x = newX, y = newY, z = newZ; }
        void Scale(Vector3 scale) { x *= scale.x, y *= scale.y, z *= scale.z; }
        constexpr bool Equals(Vector3 o) const { return x == o.x && y == o.y && z == o.z; }

        static constexpr f32 Dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
        static constexpr Vector3 Cross(Vector3 a, Vector3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
        static Vector3 Normalize(Vector3 value);
        static f32 Distance(Vector3 a, Vector3 b);
        static f32 Magnitude(Vector3 vector) { return vector.magnitude(); }
        static f32 SqrMagnitude(Vector3 vector) { return vector.sqrMagnitude(); }
        static Vector3 Lerp(Vector3 a, Vector3 b, f32 t);
        static constexpr Vector3 LerpUnclamped(Vector3 a, Vector3 b, f32 t)
        {
            return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t};
        }
        static Vector3 Slerp(Vector3 a, Vector3 b, f32 t);
        static Vector3 SlerpUnclamped(Vector3 a, Vector3 b, f32 t);
        static f32 Angle(Vector3 from, Vector3 to);
        static f32 SignedAngle(Vector3 from, Vector3 to, Vector3 axis);
        static Vector3 MoveTowards(Vector3 current, Vector3 target, f32 maxDistanceDelta);
        static Vector3 RotateTowards(Vector3 current, Vector3 target, f32 maxRadiansDelta, f32 maxMagnitudeDelta);
        static Vector3 ClampMagnitude(Vector3 vector, f32 maxLength);
        static Vector3 Project(Vector3 vector, Vector3 onNormal);
        static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal);
        static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal);
        static void OrthoNormalize(Vector3& normal, Vector3& tangent);
        static void OrthoNormalize(Vector3& normal, Vector3& tangent, Vector3& binormal);
        static Vector3 SmoothDamp(Vector3 current, Vector3 target, Vector3& currentVelocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime);
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

    inline constexpr Vector3 Vector3::zero{0, 0, 0};
    inline constexpr Vector3 Vector3::one{1, 1, 1};
    inline constexpr Vector3 Vector3::up{0, 1, 0};
    inline constexpr Vector3 Vector3::down{0, -1, 0};
    inline constexpr Vector3 Vector3::left{-1, 0, 0};
    inline constexpr Vector3 Vector3::right{1, 0, 0};
    inline constexpr Vector3 Vector3::forward{0, 0, 1};
    inline constexpr Vector3 Vector3::back{0, 0, -1};
    inline constexpr Vector3 Vector3::positiveInfinity{mathf::Infinity, mathf::Infinity, mathf::Infinity};
    inline constexpr Vector3 Vector3::negativeInfinity{mathf::NegativeInfinity, mathf::NegativeInfinity, mathf::NegativeInfinity};

    constexpr Vector3 operator*(f32 s, Vector3 v) { return v * s; }

    HX_STRUCT(Namespace = "HelixEngine")
    struct Vector4
    {
        HX_FIELD() f32 x = 0;
        HX_FIELD() f32 y = 0;
        HX_FIELD() f32 z = 0;
        HX_FIELD() f32 w = 0;

        static constexpr f32 kEpsilon = 1e-5f;
        static const Vector4 zero, one, positiveInfinity, negativeInfinity;

        constexpr Vector4() = default;
        constexpr Vector4(f32 x0, f32 y0, f32 z0, f32 w0) : x(x0), y(y0), z(z0), w(w0) {}
        constexpr Vector4(f32 x0, f32 y0, f32 z0) : x(x0), y(y0), z(z0), w(0) {}
        constexpr Vector4(f32 x0, f32 y0) : x(x0), y(y0), z(0), w(0) {}
        constexpr Vector4(Vector3 v) : x(v.x), y(v.y), z(v.z), w(0) {}
        constexpr Vector4(Vector3 v, f32 w0) : x(v.x), y(v.y), z(v.z), w(w0) {}
        constexpr Vector4(Vector2 v) : x(v.x), y(v.y), z(0), w(0) {}
        constexpr explicit operator Vector3() const { return {x, y, z}; }
        constexpr explicit operator Vector2() const { return {x, y}; }

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        constexpr f32 sqrMagnitude() const { return x * x + y * y + z * z + w * w; }
        f32 magnitude() const { return mathf::Sqrt(sqrMagnitude()); }
        Vector4 normalized() const { return Normalize(*this); }
        void Normalize() { *this = Normalize(*this); }
        void Set(f32 newX, f32 newY, f32 newZ, f32 newW) { x = newX, y = newY, z = newZ, w = newW; }
        void Scale(Vector4 scale) { x *= scale.x, y *= scale.y, z *= scale.z, w *= scale.w; }
        constexpr bool Equals(Vector4 o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }

        static constexpr f32 Dot(Vector4 a, Vector4 b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
        static Vector4 Normalize(Vector4 a);
        static f32 Distance(Vector4 a, Vector4 b);
        static Vector4 Lerp(Vector4 a, Vector4 b, f32 t);
        static constexpr Vector4 LerpUnclamped(Vector4 a, Vector4 b, f32 t)
        {
            return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t};
        }
        static Vector4 MoveTowards(Vector4 current, Vector4 target, f32 maxDistanceDelta);
        static Vector4 Project(Vector4 a, Vector4 b);
        static constexpr Vector4 Scale(Vector4 a, Vector4 b) { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }
        static Vector4 Min(Vector4 a, Vector4 b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y), mathf::Min(a.z, b.z), mathf::Min(a.w, b.w)}; }
        static Vector4 Max(Vector4 a, Vector4 b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y), mathf::Max(a.z, b.z), mathf::Max(a.w, b.w)}; }

        constexpr Vector4 operator-() const { return {-x, -y, -z, -w}; }
        constexpr Vector4 operator+(Vector4 o) const { return {x + o.x, y + o.y, z + o.z, w + o.w}; }
        constexpr Vector4 operator-(Vector4 o) const { return {x - o.x, y - o.y, z - o.z, w - o.w}; }
        constexpr Vector4 operator*(f32 s) const { return {x * s, y * s, z * s, w * s}; }
        constexpr Vector4 operator/(f32 s) const { return {x / s, y / s, z / s, w / s}; }
        Vector4& operator+=(Vector4 o) { return *this = *this + o; }
        Vector4& operator-=(Vector4 o) { return *this = *this - o; }
        Vector4& operator*=(f32 s) { return *this = *this * s; }
        Vector4& operator/=(f32 s) { return *this = *this / s; }
        constexpr bool operator==(Vector4 o) const { return (*this - o).sqrMagnitude() < kEpsilon * kEpsilon; }
    };

    inline constexpr Vector4 Vector4::zero{0, 0, 0, 0};
    inline constexpr Vector4 Vector4::one{1, 1, 1, 1};
    inline constexpr Vector4 Vector4::positiveInfinity{mathf::Infinity, mathf::Infinity, mathf::Infinity, mathf::Infinity};
    inline constexpr Vector4 Vector4::negativeInfinity{mathf::NegativeInfinity, mathf::NegativeInfinity, mathf::NegativeInfinity, mathf::NegativeInfinity};

    constexpr Vector4 operator*(f32 s, Vector4 v) { return v * s; }

    struct Vector3Int;

    // C#'s x and y are properties over the private fields m_X and m_Y.
    HX_STRUCT(Namespace = "HelixEngine")
    struct Vector2Int
    {
        HX_FIELD(Name = "m_X", Private) i32 x = 0;
        HX_FIELD(Name = "m_Y", Private) i32 y = 0;

        static const Vector2Int zero, one, up, down, left, right;

        constexpr Vector2Int() = default;
        constexpr Vector2Int(i32 x0, i32 y0) : x(x0), y(y0) {}
        constexpr operator Vector2() const { return {f32(x), f32(y)}; }
        constexpr explicit operator Vector3Int() const;

        i32& operator[](int i) { return (&x)[i]; }
        i32 operator[](int i) const { return (&x)[i]; }

        constexpr i32 sqrMagnitude() const { return x * x + y * y; }
        f32 magnitude() const { return mathf::Sqrt(f32(sqrMagnitude())); }
        void Set(i32 newX, i32 newY) { x = newX, y = newY; }
        void Scale(Vector2Int scale) { x *= scale.x, y *= scale.y; }
        void Clamp(Vector2Int min, Vector2Int max) { x = mathf::Clamp(x, min.x, max.x), y = mathf::Clamp(y, min.y, max.y); }
        constexpr bool Equals(Vector2Int o) const { return x == o.x && y == o.y; }

        static f32 Distance(Vector2Int a, Vector2Int b) { return (a - b).magnitude(); }
        static Vector2Int Min(Vector2Int a, Vector2Int b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y)}; }
        static Vector2Int Max(Vector2Int a, Vector2Int b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y)}; }
        static constexpr Vector2Int Scale(Vector2Int a, Vector2Int b) { return {a.x * b.x, a.y * b.y}; }
        static Vector2Int FloorToInt(Vector2 v) { return {mathf::FloorToInt(v.x), mathf::FloorToInt(v.y)}; }
        static Vector2Int CeilToInt(Vector2 v) { return {mathf::CeilToInt(v.x), mathf::CeilToInt(v.y)}; }
        static Vector2Int RoundToInt(Vector2 v) { return {mathf::RoundToInt(v.x), mathf::RoundToInt(v.y)}; }

        constexpr Vector2Int operator-() const { return {-x, -y}; }
        constexpr Vector2Int operator+(Vector2Int o) const { return {x + o.x, y + o.y}; }
        constexpr Vector2Int operator-(Vector2Int o) const { return {x - o.x, y - o.y}; }
        constexpr Vector2Int operator*(Vector2Int o) const { return {x * o.x, y * o.y}; }
        constexpr Vector2Int operator*(i32 s) const { return {x * s, y * s}; }
        constexpr Vector2Int operator/(i32 s) const { return {x / s, y / s}; }
        constexpr bool operator==(const Vector2Int&) const = default;
    };

    inline constexpr Vector2Int Vector2Int::zero{0, 0};
    inline constexpr Vector2Int Vector2Int::one{1, 1};
    inline constexpr Vector2Int Vector2Int::up{0, 1};
    inline constexpr Vector2Int Vector2Int::down{0, -1};
    inline constexpr Vector2Int Vector2Int::left{-1, 0};
    inline constexpr Vector2Int Vector2Int::right{1, 0};

    constexpr Vector2Int operator*(i32 s, Vector2Int v) { return v * s; }

    HX_STRUCT(Namespace = "HelixEngine")
    struct Vector3Int
    {
        HX_FIELD(Name = "m_X", Private) i32 x = 0;
        HX_FIELD(Name = "m_Y", Private) i32 y = 0;
        HX_FIELD(Name = "m_Z", Private) i32 z = 0;

        static const Vector3Int zero, one, up, down, left, right, forward, back;

        constexpr Vector3Int() = default;
        constexpr Vector3Int(i32 x0, i32 y0, i32 z0) : x(x0), y(y0), z(z0) {}
        constexpr Vector3Int(i32 x0, i32 y0) : x(x0), y(y0), z(0) {}
        constexpr operator Vector3() const { return {f32(x), f32(y), f32(z)}; }
        constexpr explicit operator Vector2Int() const { return {x, y}; }

        i32& operator[](int i) { return (&x)[i]; }
        i32 operator[](int i) const { return (&x)[i]; }

        constexpr i32 sqrMagnitude() const { return x * x + y * y + z * z; }
        f32 magnitude() const { return mathf::Sqrt(f32(sqrMagnitude())); }
        void Set(i32 newX, i32 newY, i32 newZ) { x = newX, y = newY, z = newZ; }
        void Scale(Vector3Int scale) { x *= scale.x, y *= scale.y, z *= scale.z; }
        void Clamp(Vector3Int min, Vector3Int max)
        {
            x = mathf::Clamp(x, min.x, max.x), y = mathf::Clamp(y, min.y, max.y), z = mathf::Clamp(z, min.z, max.z);
        }
        constexpr bool Equals(Vector3Int o) const { return x == o.x && y == o.y && z == o.z; }

        static f32 Distance(Vector3Int a, Vector3Int b) { return (a - b).magnitude(); }
        static Vector3Int Min(Vector3Int a, Vector3Int b) { return {mathf::Min(a.x, b.x), mathf::Min(a.y, b.y), mathf::Min(a.z, b.z)}; }
        static Vector3Int Max(Vector3Int a, Vector3Int b) { return {mathf::Max(a.x, b.x), mathf::Max(a.y, b.y), mathf::Max(a.z, b.z)}; }
        static constexpr Vector3Int Scale(Vector3Int a, Vector3Int b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
        static Vector3Int FloorToInt(Vector3 v) { return {mathf::FloorToInt(v.x), mathf::FloorToInt(v.y), mathf::FloorToInt(v.z)}; }
        static Vector3Int CeilToInt(Vector3 v) { return {mathf::CeilToInt(v.x), mathf::CeilToInt(v.y), mathf::CeilToInt(v.z)}; }
        static Vector3Int RoundToInt(Vector3 v) { return {mathf::RoundToInt(v.x), mathf::RoundToInt(v.y), mathf::RoundToInt(v.z)}; }

        constexpr Vector3Int operator-() const { return {-x, -y, -z}; }
        constexpr Vector3Int operator+(Vector3Int o) const { return {x + o.x, y + o.y, z + o.z}; }
        constexpr Vector3Int operator-(Vector3Int o) const { return {x - o.x, y - o.y, z - o.z}; }
        constexpr Vector3Int operator*(Vector3Int o) const { return {x * o.x, y * o.y, z * o.z}; }
        constexpr Vector3Int operator*(i32 s) const { return {x * s, y * s, z * s}; }
        constexpr Vector3Int operator/(i32 s) const { return {x / s, y / s, z / s}; }
        constexpr bool operator==(const Vector3Int&) const = default;
    };

    inline constexpr Vector3Int Vector3Int::zero{0, 0, 0};
    inline constexpr Vector3Int Vector3Int::one{1, 1, 1};
    inline constexpr Vector3Int Vector3Int::up{0, 1, 0};
    inline constexpr Vector3Int Vector3Int::down{0, -1, 0};
    inline constexpr Vector3Int Vector3Int::left{-1, 0, 0};
    inline constexpr Vector3Int Vector3Int::right{1, 0, 0};
    inline constexpr Vector3Int Vector3Int::forward{0, 0, 1};
    inline constexpr Vector3Int Vector3Int::back{0, 0, -1};

    constexpr Vector3Int operator*(i32 s, Vector3Int v) { return v * s; }

    constexpr Vector2Int::operator Vector3Int() const { return {x, y, 0}; }
}
