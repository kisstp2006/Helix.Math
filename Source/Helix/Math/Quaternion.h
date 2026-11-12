#pragma once

#include <Helix/Math/Vector.h>

namespace helix
{
    // == compares approximately (dot above 1 - 1e-6), Equals exactly. A default Quaternion is the identity.
    HX_STRUCT(Namespace = "HelixEngine")
    struct Quaternion
    {
        HX_FIELD() f32 x = 0;
        HX_FIELD() f32 y = 0;
        HX_FIELD() f32 z = 0;
        HX_FIELD() f32 w = 1;

        static constexpr f32 kEpsilon = 1e-6f;
        static const Quaternion identity;

        constexpr Quaternion() = default;
        constexpr Quaternion(f32 x0, f32 y0, f32 z0, f32 w0) : x(x0), y(y0), z(z0), w(w0) {}

        f32& operator[](int i) { return (&x)[i]; }
        f32 operator[](int i) const { return (&x)[i]; }

        Vector3 eulerAngles() const;
        void SetEulerAngles(Vector3 euler) { *this = Euler(euler); }
        Quaternion normalized() const { return Normalize(*this); }
        void Normalize() { *this = Normalize(*this); }
        void Set(f32 newX, f32 newY, f32 newZ, f32 newW) { x = newX, y = newY, z = newZ, w = newW; }
        void SetFromToRotation(Vector3 fromDirection, Vector3 toDirection) { *this = FromToRotation(fromDirection, toDirection); }
        void SetLookRotation(Vector3 view, Vector3 up = Vector3::up) { *this = LookRotation(view, up); }
        constexpr bool Equals(Quaternion o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }

        static Quaternion Euler(f32 ex, f32 ey, f32 ez);
        static Quaternion Euler(Vector3 degrees) { return Euler(degrees.x, degrees.y, degrees.z); }
        static Quaternion AngleAxis(f32 degrees, Vector3 axis);
        static Quaternion LookRotation(Vector3 forward, Vector3 upwards = Vector3::up);
        static Quaternion FromToRotation(Vector3 from, Vector3 to);
        static Quaternion Inverse(Quaternion q) { return {-q.x, -q.y, -q.z, q.w}; }
        static Quaternion Normalize(Quaternion q);
        static constexpr f32 Dot(Quaternion a, Quaternion b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
        static f32 Angle(Quaternion a, Quaternion b);
        static Quaternion Slerp(Quaternion a, Quaternion b, f32 t);
        static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, f32 t);
        static Quaternion Lerp(Quaternion a, Quaternion b, f32 t);
        static Quaternion LerpUnclamped(Quaternion a, Quaternion b, f32 t);
        static Quaternion RotateTowards(Quaternion from, Quaternion to, f32 maxDegrees);
        void ToAngleAxis(f32& degrees, Vector3& axis) const;

        constexpr Quaternion operator*(Quaternion b) const
        {
            return {w * b.x + x * b.w + y * b.z - z * b.y,
                    w * b.y + y * b.w + z * b.x - x * b.z,
                    w * b.z + z * b.w + x * b.y - y * b.x,
                    w * b.w - x * b.x - y * b.y - z * b.z};
        }

        constexpr Vector3 operator*(Vector3 v) const
        {
            f32 x2 = x * 2, y2 = y * 2, z2 = z * 2;
            f32 xx = x * x2, yy = y * y2, zz = z * z2, xy = x * y2, xz = x * z2, yz = y * z2;
            f32 wx = w * x2, wy = w * y2, wz = w * z2;
            return {(1 - (yy + zz)) * v.x + (xy - wz) * v.y + (xz + wy) * v.z,
                    (xy + wz) * v.x + (1 - (xx + zz)) * v.y + (yz - wx) * v.z,
                    (xz - wy) * v.x + (yz + wx) * v.y + (1 - (xx + yy)) * v.z};
        }

        constexpr bool operator==(Quaternion o) const { return Dot(*this, o) > 1.0f - kEpsilon; }
    };

    inline constexpr Quaternion Quaternion::identity{0, 0, 0, 1};
}
