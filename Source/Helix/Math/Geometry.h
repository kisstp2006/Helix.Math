#pragma once

#include <Helix/Core/Basics/Span.h>
#include <Helix/Math/Matrix4x4.h>

namespace helix
{
    struct Rect
    {
        f32 x = 0, y = 0, width = 0, height = 0;

        constexpr f32 xMin() const { return x; }
        constexpr f32 yMin() const { return y; }
        constexpr f32 xMax() const { return x + width; }
        constexpr f32 yMax() const { return y + height; }
        constexpr Vector2 center() const { return {x + width * 0.5f, y + height * 0.5f}; }
        constexpr Vector2 size() const { return {width, height}; }
        static constexpr Rect MinMaxRect(f32 xmin, f32 ymin, f32 xmax, f32 ymax) { return {xmin, ymin, xmax - xmin, ymax - ymin}; }

        bool Contains(Vector2 p) const;
        bool Overlaps(const Rect& o) const;
        constexpr bool operator==(const Rect&) const = default;
    };

    struct RectInt
    {
        i32 x = 0, y = 0, width = 0, height = 0;

        constexpr i32 xMin() const { return x; }
        constexpr i32 yMin() const { return y; }
        constexpr i32 xMax() const { return x + width; }
        constexpr i32 yMax() const { return y + height; }
        constexpr Vector2Int position() const { return {x, y}; }
        constexpr Vector2Int size() const { return {width, height}; }
        constexpr bool Contains(Vector2Int p) const { return p.x >= x && p.y >= y && p.x < x + width && p.y < y + height; }
        constexpr bool Overlaps(const RectInt& o) const { return o.x < x + width && o.x + o.width > x && o.y < y + height && o.y + o.height > y; }
        constexpr bool operator==(const RectInt&) const = default;
    };

    struct Bounds
    {
        Vector3 center;
        Vector3 extents;

        constexpr Bounds() = default;
        constexpr Bounds(Vector3 c, Vector3 size) : center(c), extents(size * 0.5f) {}

        constexpr Vector3 size() const { return extents * 2.0f; }
        constexpr Vector3 min() const { return center - extents; }
        constexpr Vector3 max() const { return center + extents; }
        void SetMinMax(Vector3 mn, Vector3 mx);
        void Encapsulate(Vector3 point);
        void Encapsulate(const Bounds& b);
        void Expand(f32 amount) { extents += Vector3(amount, amount, amount) * 0.5f; }
        bool Contains(Vector3 p) const;
        bool Intersects(const Bounds& b) const;
        Vector3 ClosestPoint(Vector3 p) const;
        f32 SqrDistance(Vector3 p) const;
        bool operator==(const Bounds& o) const { return center == o.center && extents == o.extents; }
    };

    struct Ray
    {
        Vector3 origin;
        Vector3 direction;

        Ray() = default;
        Ray(Vector3 o, Vector3 d) : origin(o), direction(Vector3::Normalize(d)) {}
        constexpr Vector3 GetPoint(f32 distance) const { return origin + direction * distance; }
    };

    struct Plane
    {
        Vector3 normal;
        f32 distance = 0;

        Plane() = default;
        Plane(Vector3 inNormal, f32 d) : normal(Vector3::Normalize(inNormal)), distance(d) {}
        Plane(Vector3 inNormal, Vector3 point) : normal(Vector3::Normalize(inNormal)), distance(-Vector3::Dot(normal, point)) {}
        Plane(Vector3 a, Vector3 b, Vector3 c);

        f32 GetDistanceToPoint(Vector3 p) const { return Vector3::Dot(normal, p) + distance; }
        bool GetSide(Vector3 p) const { return GetDistanceToPoint(p) > 0.0f; }
        Vector3 ClosestPointOnPlane(Vector3 p) const { return p - normal * GetDistanceToPoint(p); }
        bool Raycast(const Ray& ray, f32& enter) const;
    };

    bool IntersectRay(const Bounds& bounds, const Ray& ray, f32& distance);

    // Order: left, right, bottom, top, near, far.
    void CalculateFrustumPlanes(const Matrix4x4& worldToProjection, Plane out[6]);
    bool TestPlanesAABB(const Plane planes[6], const Bounds& bounds);

    struct BoundsSoA
    {
        const f32* centerX;
        const f32* centerY;
        const f32* centerZ;
        const f32* extentX;
        const f32* extentY;
        const f32* extentZ;
        usize count;
    };

    void CullBoundsSoA(const Plane planes[6], const BoundsSoA& bounds, u8* visible);
    void TransformPoints(const Matrix4x4& m, Span<const Vector3> points, Span<Vector3> out);

    // Same results split over the job system in batches; they run on the calling thread when the job system is not
    // initialized or the input is small. Both return after the work is complete.
    void CullBoundsSoAParallel(const Plane planes[6], const BoundsSoA& bounds, u8* visible, u32 batchSize = 4096);
    void TransformPointsParallel(const Matrix4x4& m, Span<const Vector3> points, Span<Vector3> out, u32 batchSize = 4096);
}
