#include <Helix/Math/Quaternion.h>

namespace helix
{
    using namespace mathf;

    namespace
    {
        Quaternion FromBasis(Vector3 r, Vector3 u, Vector3 f)
        {
            f32 m00 = r.x, m01 = u.x, m02 = f.x;
            f32 m10 = r.y, m11 = u.y, m12 = f.y;
            f32 m20 = r.z, m21 = u.z, m22 = f.z;
            f32 trace = m00 + m11 + m22;
            Quaternion q;
            if (trace > 0.0f)
            {
                f32 s = mathf::Sqrt(trace + 1.0f) * 2.0f;
                q = {(m21 - m12) / s, (m02 - m20) / s, (m10 - m01) / s, 0.25f * s};
            }
            else if (m00 > m11 && m00 > m22)
            {
                f32 s = mathf::Sqrt(1.0f + m00 - m11 - m22) * 2.0f;
                q = {0.25f * s, (m01 + m10) / s, (m02 + m20) / s, (m21 - m12) / s};
            }
            else if (m11 > m22)
            {
                f32 s = mathf::Sqrt(1.0f + m11 - m00 - m22) * 2.0f;
                q = {(m01 + m10) / s, 0.25f * s, (m12 + m21) / s, (m02 - m20) / s};
            }
            else
            {
                f32 s = mathf::Sqrt(1.0f + m22 - m00 - m11) * 2.0f;
                q = {(m02 + m20) / s, (m12 + m21) / s, 0.25f * s, (m10 - m01) / s};
            }
            return Quaternion::Normalize(q);
        }

        f32 PositiveDegrees(f32 radians)
        {
            f32 d = radians * Rad2Deg;
            d = Repeat(d, 360.0f);
            return d >= 360.0f ? 0.0f : d;
        }
    }

    Quaternion Quaternion::Euler(f32 ex, f32 ey, f32 ez)
    {
        return AngleAxis(ey, Vector3::up) * AngleAxis(ex, Vector3::right) * AngleAxis(ez, Vector3::forward);
    }

    Vector3 Quaternion::eulerAngles() const
    {
        Quaternion q = Normalize(*this);
        f32 m12 = 2 * (q.y * q.z - q.w * q.x);
        f32 sx = Clamp(-m12, -1.0f, 1.0f);
        f32 ax = Asin(sx);
        f32 ay, az;
        if (Abs(sx) < 0.9999f)
        {
            f32 m02 = 2 * (q.x * q.z + q.w * q.y);
            f32 m22 = 1 - 2 * (q.x * q.x + q.y * q.y);
            f32 m10 = 2 * (q.x * q.y + q.w * q.z);
            f32 m11 = 1 - 2 * (q.x * q.x + q.z * q.z);
            ay = Atan2(m02, m22);
            az = Atan2(m10, m11);
        }
        else
        {
            f32 m20 = 2 * (q.x * q.z - q.w * q.y);
            f32 m00 = 1 - 2 * (q.y * q.y + q.z * q.z);
            ay = Atan2(-m20, m00);
            az = 0.0f;
        }
        return {PositiveDegrees(ax), PositiveDegrees(ay), PositiveDegrees(az)};
    }

    Quaternion Quaternion::AngleAxis(f32 degrees, Vector3 axis)
    {
        Vector3 n = Vector3::Normalize(axis);
        if (n.sqrMagnitude() == 0.0f)
            return identity;
        f32 half = degrees * Deg2Rad * 0.5f;
        f32 s = Sin(half);
        return {n.x * s, n.y * s, n.z * s, Cos(half)};
    }

    void Quaternion::ToAngleAxis(f32& degrees, Vector3& axis) const
    {
        Quaternion q = Normalize(*this);
        if (q.w < 0.0f)
            q = {-q.x, -q.y, -q.z, -q.w};
        degrees = 2.0f * Acos(Clamp(q.w, -1.0f, 1.0f)) * Rad2Deg;
        f32 s = mathf::Sqrt(mathf::Max(0.0f, 1.0f - q.w * q.w));
        axis = s > 1e-6f ? Vector3(q.x / s, q.y / s, q.z / s) : Vector3::right;
    }

    Quaternion Quaternion::LookRotation(Vector3 forward, Vector3 up)
    {
        Vector3 f = Vector3::Normalize(forward);
        if (f.sqrMagnitude() == 0.0f)
            return identity;
        Vector3 r = Vector3::Cross(up, f);
        if (r.sqrMagnitude() < 1e-12f)
            r = Vector3::Cross(Abs(f.y) < 0.99f ? Vector3::up : Vector3::forward, f);
        r = Vector3::Normalize(r);
        Vector3 u = Vector3::Cross(f, r);
        return FromBasis(r, u, f);
    }

    Quaternion Quaternion::FromToRotation(Vector3 from, Vector3 to)
    {
        Vector3 a = Vector3::Normalize(from), b = Vector3::Normalize(to);
        f32 dot = Vector3::Dot(a, b);
        if (dot >= 1.0f - 1e-6f)
            return identity;
        if (dot <= -1.0f + 1e-6f)
        {
            Vector3 axis = Vector3::Cross(Vector3::right, a);
            if (axis.sqrMagnitude() < 1e-12f)
                axis = Vector3::Cross(Vector3::up, a);
            return AngleAxis(180.0f, axis);
        }
        Vector3 c = Vector3::Cross(a, b);
        return Normalize({c.x, c.y, c.z, 1.0f + dot});
    }

    Quaternion Quaternion::Normalize(Quaternion q)
    {
        f32 m = mathf::Sqrt(Dot(q, q));
        if (m < Epsilon)
            return identity;
        return {q.x / m, q.y / m, q.z / m, q.w / m};
    }

    f32 Quaternion::Angle(Quaternion a, Quaternion b)
    {
        f32 dot = mathf::Min(Abs(Dot(a, b)), 1.0f);
        return dot > 1.0f - kEpsilon ? 0.0f : Acos(dot) * 2.0f * Rad2Deg;
    }

    Quaternion Quaternion::SlerpUnclamped(Quaternion a, Quaternion b, f32 t)
    {
        f32 dot = Dot(a, b);
        if (dot < 0.0f)
        {
            b = {-b.x, -b.y, -b.z, -b.w};
            dot = -dot;
        }
        if (dot > 0.9995f)
            return LerpUnclamped(a, b, t);
        f32 theta = Acos(Clamp(dot, -1.0f, 1.0f));
        f32 s = Sin(theta);
        f32 wa = Sin((1.0f - t) * theta) / s, wb = Sin(t * theta) / s;
        return Normalize({a.x * wa + b.x * wb, a.y * wa + b.y * wb, a.z * wa + b.z * wb, a.w * wa + b.w * wb});
    }

    Quaternion Quaternion::Slerp(Quaternion a, Quaternion b, f32 t) { return SlerpUnclamped(a, b, Clamp01(t)); }

    Quaternion Quaternion::LerpUnclamped(Quaternion a, Quaternion b, f32 t)
    {
        if (Dot(a, b) < 0.0f)
            b = {-b.x, -b.y, -b.z, -b.w};
        return Normalize({a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t});
    }

    Quaternion Quaternion::Lerp(Quaternion a, Quaternion b, f32 t) { return LerpUnclamped(a, b, Clamp01(t)); }

    Quaternion Quaternion::RotateTowards(Quaternion from, Quaternion to, f32 maxDegrees)
    {
        f32 angle = Angle(from, to);
        if (angle == 0.0f)
            return to;
        return SlerpUnclamped(from, to, mathf::Min(1.0f, maxDegrees / angle));
    }
}
