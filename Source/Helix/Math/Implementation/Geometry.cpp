#include <Helix/Math/Geometry.h>
#include <Helix/Math/Simd.h>

namespace helix
{
    using namespace mathf;

    bool Rect::Contains(Vector2 p) const { return p.x >= xMin() && p.x < xMax() && p.y >= yMin() && p.y < yMax(); }

    bool Rect::Overlaps(const Rect& o) const
    {
        return o.xMax() > xMin() && o.xMin() < xMax() && o.yMax() > yMin() && o.yMin() < yMax();
    }

    void Bounds::SetMinMax(Vector3 mn, Vector3 mx)
    {
        extents = (mx - mn) * 0.5f;
        center = mn + extents;
    }

    void Bounds::Encapsulate(Vector3 p) { SetMinMax(Vector3::Min(min(), p), Vector3::Max(max(), p)); }

    void Bounds::Encapsulate(const Bounds& b)
    {
        Encapsulate(b.center - b.extents);
        Encapsulate(b.center + b.extents);
    }

    bool Bounds::Contains(Vector3 p) const
    {
        Vector3 mn = min(), mx = max();
        return p.x >= mn.x && p.x <= mx.x && p.y >= mn.y && p.y <= mx.y && p.z >= mn.z && p.z <= mx.z;
    }

    bool Bounds::Intersects(const Bounds& b) const
    {
        Vector3 amn = min(), amx = max(), bmn = b.min(), bmx = b.max();
        return amn.x <= bmx.x && amx.x >= bmn.x && amn.y <= bmx.y && amx.y >= bmn.y && amn.z <= bmx.z && amx.z >= bmn.z;
    }

    Vector3 Bounds::ClosestPoint(Vector3 p) const { return Vector3::Min(Vector3::Max(p, min()), max()); }
    f32 Bounds::SqrDistance(Vector3 p) const { return (ClosestPoint(p) - p).sqrMagnitude(); }

    Plane::Plane(Vector3 a, Vector3 b, Vector3 c)
        : normal(Vector3::Normalize(Vector3::Cross(b - a, c - a))), distance(-Vector3::Dot(normal, a))
    {
    }

    bool Plane::Raycast(const Ray& ray, f32& enter) const
    {
        f32 vdot = Vector3::Dot(ray.direction, normal);
        f32 ndot = -Vector3::Dot(ray.origin, normal) - distance;
        if (Approximately(vdot, 0.0f))
        {
            enter = 0.0f;
            return false;
        }
        enter = ndot / vdot;
        return enter > 0.0f;
    }

    bool IntersectRay(const Bounds& bounds, const Ray& ray, f32& distance)
    {
        f32 tmin = NegativeInfinity, tmax = Infinity;
        Vector3 mn = bounds.min(), mx = bounds.max();
        for (int i = 0; i < 3; i++)
        {
            if (Abs(ray.direction[i]) < 1e-8f)
            {
                if (ray.origin[i] < mn[i] || ray.origin[i] > mx[i])
                    return false;
                continue;
            }
            f32 inv = 1.0f / ray.direction[i];
            f32 t1 = (mn[i] - ray.origin[i]) * inv, t2 = (mx[i] - ray.origin[i]) * inv;
            if (t1 > t2)
            {
                f32 t = t1;
                t1 = t2;
                t2 = t;
            }
            tmin = mathf::Max(tmin, t1);
            tmax = mathf::Min(tmax, t2);
            if (tmin > tmax)
                return false;
        }
        if (tmax < 0.0f)
            return false;
        distance = tmin > 0.0f ? tmin : 0.0f;
        return true;
    }

    void CalculateFrustumPlanes(const Matrix4x4& m, Plane out[6])
    {
        Vector4 r0 = m.GetRow(0), r1 = m.GetRow(1), r2 = m.GetRow(2), r3 = m.GetRow(3);
        Vector4 p[6] = {r3 + r0, r3 - r0, r3 + r1, r3 - r1, r3 + r2, r3 - r2};
        for (int i = 0; i < 6; i++)
        {
            Vector3 n(p[i].x, p[i].y, p[i].z);
            f32 len = n.magnitude();
            out[i].normal = n / len;
            out[i].distance = p[i].w / len;
        }
    }

    bool TestPlanesAABB(const Plane planes[6], const Bounds& b)
    {
        for (int i = 0; i < 6; i++)
        {
            const Vector3& n = planes[i].normal;
            f32 r = b.extents.x * Abs(n.x) + b.extents.y * Abs(n.y) + b.extents.z * Abs(n.z);
            if (planes[i].GetDistanceToPoint(b.center) < -r)
                return false;
        }
        return true;
    }

    void CullBoundsSoA(const Plane planes[6], const BoundsSoA& s, u8* visible)
    {
        usize i = 0;
        for (; i + 4 <= s.count; i += 4)
        {
            simd::float4 cx = simd::Load(s.centerX + i), cy = simd::Load(s.centerY + i), cz = simd::Load(s.centerZ + i);
            simd::float4 ex = simd::Load(s.extentX + i), ey = simd::Load(s.extentY + i), ez = simd::Load(s.extentZ + i);
            int outside = 0;
            for (int p = 0; p < 6; p++)
            {
                const Plane& pl = planes[p];
                simd::float4 d = simd::MulAdd(cx, simd::Splat(pl.normal.x), simd::Splat(pl.distance));
                d = simd::MulAdd(cy, simd::Splat(pl.normal.y), d);
                d = simd::MulAdd(cz, simd::Splat(pl.normal.z), d);
                simd::float4 r = simd::Mul(ex, simd::Splat(Abs(pl.normal.x)));
                r = simd::MulAdd(ey, simd::Splat(Abs(pl.normal.y)), r);
                r = simd::MulAdd(ez, simd::Splat(Abs(pl.normal.z)), r);
                outside |= simd::MoveMask(simd::Less(simd::Add(d, r), simd::Splat(0.0f)));
            }
            for (int k = 0; k < 4; k++)
                visible[i + usize(k)] = u8(((outside >> k) & 1) == 0);
        }
        for (; i < s.count; i++)
        {
            Bounds b;
            b.center = {s.centerX[i], s.centerY[i], s.centerZ[i]};
            b.extents = {s.extentX[i], s.extentY[i], s.extentZ[i]};
            visible[i] = u8(TestPlanesAABB(planes, b));
        }
    }

    void TransformPoints(const Matrix4x4& m, Span<const Vector3> points, Span<Vector3> out)
    {
        HX_ASSERT(out.Size() >= points.Size());
        simd::float4 c0 = simd::Load(&m.m00), c1 = simd::Load(&m.m01), c2 = simd::Load(&m.m02), c3 = simd::Load(&m.m03);
        for (usize i = 0; i < points.Size(); i++)
        {
            const Vector3& p = points[i];
            simd::float4 v = simd::MulAdd(c0, simd::Splat(p.x), c3);
            v = simd::MulAdd(c1, simd::Splat(p.y), v);
            v = simd::MulAdd(c2, simd::Splat(p.z), v);
            f32 r[4];
            simd::Store(r, v);
            out[i] = {r[0], r[1], r[2]};
        }
    }
}
