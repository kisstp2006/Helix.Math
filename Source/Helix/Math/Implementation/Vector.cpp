#include <Helix/Math/Vector.h>

namespace helix
{
    using namespace mathf;

    Vector2 Vector2::normalized() const
    {
        f32 m = magnitude();
        return m > kEpsilon ? *this / m : zero();
    }

    f32 Vector2::Distance(Vector2 a, Vector2 b) { return (a - b).magnitude(); }
    Vector2 Vector2::Lerp(Vector2 a, Vector2 b, f32 t) { return LerpUnclamped(a, b, Clamp01(t)); }

    f32 Vector2::Angle(Vector2 from, Vector2 to)
    {
        f32 d = mathf::Sqrt(from.sqrMagnitude() * to.sqrMagnitude());
        if (d < kEpsilonNormalSqrt)
            return 0.0f;
        return Acos(Clamp(Dot(from, to) / d, -1.0f, 1.0f)) * Rad2Deg;
    }

    f32 Vector2::SignedAngle(Vector2 from, Vector2 to)
    {
        return Angle(from, to) * Sign(from.x * to.y - from.y * to.x);
    }

    Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, f32 maxDistance)
    {
        Vector2 d = target - current;
        f32 sq = d.sqrMagnitude();
        if (sq == 0.0f || (maxDistance >= 0.0f && sq <= maxDistance * maxDistance))
            return target;
        return current + d / mathf::Sqrt(sq) * maxDistance;
    }

    Vector2 Vector2::ClampMagnitude(Vector2 v, f32 maxLength)
    {
        f32 sq = v.sqrMagnitude();
        if (sq > maxLength * maxLength)
            return v / mathf::Sqrt(sq) * maxLength;
        return v;
    }

    Vector2 Vector2::Reflect(Vector2 direction, Vector2 normal) { return direction - normal * (2.0f * Dot(normal, direction)); }

    Vector3 Vector3::Normalize(Vector3 v)
    {
        f32 m = v.magnitude();
        return m > kEpsilon ? v / m : zero();
    }

    f32 Vector3::Distance(Vector3 a, Vector3 b) { return (a - b).magnitude(); }
    Vector3 Vector3::Lerp(Vector3 a, Vector3 b, f32 t) { return LerpUnclamped(a, b, Clamp01(t)); }

    f32 Vector3::Angle(Vector3 from, Vector3 to)
    {
        f32 d = mathf::Sqrt(from.sqrMagnitude() * to.sqrMagnitude());
        if (d < kEpsilonNormalSqrt)
            return 0.0f;
        return Acos(Clamp(Dot(from, to) / d, -1.0f, 1.0f)) * Rad2Deg;
    }

    f32 Vector3::SignedAngle(Vector3 from, Vector3 to, Vector3 axis)
    {
        return Angle(from, to) * Sign(Dot(axis, Cross(from, to)));
    }

    Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, f32 maxDistance)
    {
        Vector3 d = target - current;
        f32 sq = d.sqrMagnitude();
        if (sq == 0.0f || (maxDistance >= 0.0f && sq <= maxDistance * maxDistance))
            return target;
        return current + d / mathf::Sqrt(sq) * maxDistance;
    }

    Vector3 Vector3::ClampMagnitude(Vector3 v, f32 maxLength)
    {
        f32 sq = v.sqrMagnitude();
        if (sq > maxLength * maxLength)
            return v / mathf::Sqrt(sq) * maxLength;
        return v;
    }

    Vector3 Vector3::Project(Vector3 v, Vector3 normal)
    {
        f32 sq = Dot(normal, normal);
        if (sq < Epsilon)
            return zero();
        return normal * (Dot(v, normal) / sq);
    }

    Vector3 Vector3::ProjectOnPlane(Vector3 v, Vector3 planeNormal) { return v - Project(v, planeNormal); }
    Vector3 Vector3::Reflect(Vector3 direction, Vector3 normal) { return direction - normal * (2.0f * Dot(normal, direction)); }

    void Vector3::OrthoNormalize(Vector3& normal, Vector3& tangent)
    {
        normal = Normalize(normal);
        tangent = Normalize(tangent - Project(tangent, normal));
        if (tangent.sqrMagnitude() == 0.0f)
        {
            Vector3 axis = Abs(normal.x) < 0.9f ? right() : up();
            tangent = Normalize(Cross(normal, axis));
        }
    }

    Vector3 Vector3::Slerp(Vector3 a, Vector3 b, f32 t)
    {
        t = Clamp01(t);
        f32 ma = a.magnitude(), mb = b.magnitude();
        if (ma < kEpsilon || mb < kEpsilon)
            return LerpUnclamped(a, b, t);
        Vector3 na = a / ma, nb = b / mb;
        f32 dot = Clamp(Dot(na, nb), -1.0f, 1.0f);
        f32 theta = Acos(dot) * t;
        Vector3 rel = nb - na * dot;
        if (rel.sqrMagnitude() < kEpsilonNormalSqrt)
        {
            if (dot > 0.0f)
                return LerpUnclamped(a, b, t);
            Vector3 n = na;
            rel = Abs(na.x) < 0.9f ? right() : up();
            OrthoNormalize(n, rel);
        }
        else
        {
            rel = Normalize(rel);
        }
        return (na * Cos(theta) + rel * Sin(theta)) * mathf::LerpUnclamped(ma, mb, t);
    }

    Vector3 Vector3::RotateTowards(Vector3 current, Vector3 target, f32 maxRadians, f32 maxMagnitude)
    {
        f32 mc = current.magnitude(), mt = target.magnitude();
        if (mc < kEpsilon || mt < kEpsilon)
            return MoveTowards(current, target, maxMagnitude);
        f32 angle = Angle(current, target) * Deg2Rad;
        f32 t = angle > 0.0f ? mathf::Min(1.0f, maxRadians / angle) : 1.0f;
        Vector3 direction = Slerp(current / mc, target / mt, t);
        return Normalize(direction) * mathf::MoveTowards(mc, mt, maxMagnitude);
    }

    Vector3 Vector3::SmoothDamp(Vector3 current, Vector3 target, Vector3& velocity, f32 smoothTime, f32 maxSpeed, f32 deltaTime)
    {
        smoothTime = mathf::Max(0.0001f, smoothTime);
        f32 omega = 2.0f / smoothTime;
        f32 x = omega * deltaTime;
        f32 decay = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        Vector3 change = ClampMagnitude(current - target, maxSpeed * smoothTime);
        Vector3 originalTarget = target;
        target = current - change;
        Vector3 temp = (velocity + change * omega) * deltaTime;
        velocity = (velocity - temp * omega) * decay;
        Vector3 output = target + (change + temp) * decay;
        if (Dot(originalTarget - current, output - originalTarget) > 0.0f)
        {
            output = originalTarget;
            velocity = (output - originalTarget) / deltaTime;
        }
        return output;
    }

    Vector4 Vector4::normalized() const
    {
        f32 m = magnitude();
        return m > kEpsilon ? *this / m : zero();
    }

    Vector4 Vector4::Lerp(Vector4 a, Vector4 b, f32 t)
    {
        t = Clamp01(t);
        return a + (b - a) * t;
    }
}
