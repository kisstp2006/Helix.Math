#pragma once

#include <Helix/Core/Basics/Span.h>
#include <Helix/Math/Matrix4x4.h>

namespace helix
{
    // x, y, width and height are the fields (C#: properties over m_XMin, m_YMin, m_Width, m_Height); setting xMin
    // or the others moves that edge and keeps the opposite one.
    HX_STRUCT(Namespace = "HelixEngine")
    struct Rect
    {
        HX_FIELD(Name = "m_XMin", Private) f32 x = 0;
        HX_FIELD(Name = "m_YMin", Private) f32 y = 0;
        HX_FIELD(Name = "m_Width", Private) f32 width = 0;
        HX_FIELD(Name = "m_Height", Private) f32 height = 0;

        static const Rect zero;

        constexpr Rect() = default;
        constexpr Rect(f32 x0, f32 y0, f32 w, f32 h) : x(x0), y(y0), width(w), height(h) {}
        constexpr Rect(Vector2 position, Vector2 size) : x(position.x), y(position.y), width(size.x), height(size.y) {}

        constexpr f32 xMin() const { return x; }
        constexpr f32 yMin() const { return y; }
        constexpr f32 xMax() const { return x + width; }
        constexpr f32 yMax() const { return y + height; }
        constexpr Vector2 position() const { return {x, y}; }
        constexpr Vector2 size() const { return {width, height}; }
        constexpr Vector2 center() const { return {x + width * 0.5f, y + height * 0.5f}; }
        constexpr Vector2 min() const { return {xMin(), yMin()}; }
        constexpr Vector2 max() const { return {xMax(), yMax()}; }
        void SetXMin(f32 v) { f32 m = xMax(); x = v, width = m - v; }
        void SetYMin(f32 v) { f32 m = yMax(); y = v, height = m - v; }
        void SetXMax(f32 v) { width = v - x; }
        void SetYMax(f32 v) { height = v - y; }
        void SetPosition(Vector2 v) { x = v.x, y = v.y; }
        void SetSize(Vector2 v) { width = v.x, height = v.y; }
        void SetCenter(Vector2 v) { x = v.x - width * 0.5f, y = v.y - height * 0.5f; }
        void SetMin(Vector2 v) { SetXMin(v.x), SetYMin(v.y); }
        void SetMax(Vector2 v) { SetXMax(v.x), SetYMax(v.y); }
        void Set(f32 x0, f32 y0, f32 w, f32 h) { x = x0, y = y0, width = w, height = h; }

        static constexpr Rect MinMaxRect(f32 xmin, f32 ymin, f32 xmax, f32 ymax) { return {xmin, ymin, xmax - xmin, ymax - ymin}; }
        static Vector2 NormalizedToPoint(const Rect& rectangle, Vector2 normalizedRectCoordinates);
        static Vector2 PointToNormalized(const Rect& rectangle, Vector2 point);

        // The point's x and y; with allowInverse a negative width or height counts too.
        bool Contains(Vector2 point) const;
        bool Contains(Vector3 point) const { return Contains(Vector2(point)); }
        bool Contains(Vector3 point, bool allowInverse) const;
        bool Overlaps(const Rect& other) const;
        bool Overlaps(const Rect& other, bool allowInverse) const;
        constexpr bool Equals(const Rect& o) const { return x == o.x && y == o.y && width == o.width && height == o.height; }
        constexpr bool operator==(const Rect&) const = default;
    };

    inline constexpr Rect Rect::zero{0, 0, 0, 0};

    // xMin is inclusive, xMax exclusive (as y); positions are cells.
    HX_STRUCT(Namespace = "HelixEngine")
    struct RectInt
    {
        HX_FIELD(Name = "m_XMin", Private) i32 x = 0;
        HX_FIELD(Name = "m_YMin", Private) i32 y = 0;
        HX_FIELD(Name = "m_Width", Private) i32 width = 0;
        HX_FIELD(Name = "m_Height", Private) i32 height = 0;

        static const RectInt zero;

        constexpr RectInt() = default;
        constexpr RectInt(i32 x0, i32 y0, i32 w, i32 h) : x(x0), y(y0), width(w), height(h) {}
        constexpr RectInt(Vector2Int position, Vector2Int size) : x(position.x), y(position.y), width(size.x), height(size.y) {}

        constexpr i32 xMin() const { return width < 0 ? x + width : x; }
        constexpr i32 yMin() const { return height < 0 ? y + height : y; }
        constexpr i32 xMax() const { return width < 0 ? x : x + width; }
        constexpr i32 yMax() const { return height < 0 ? y : y + height; }
        constexpr Vector2Int position() const { return {x, y}; }
        constexpr Vector2Int size() const { return {width, height}; }
        constexpr Vector2 center() const { return {f32(x) + f32(width) * 0.5f, f32(y) + f32(height) * 0.5f}; }
        constexpr Vector2Int min() const { return {xMin(), yMin()}; }
        constexpr Vector2Int max() const { return {xMax(), yMax()}; }
        void SetXMin(i32 v) { i32 m = xMax(); x = v, width = m - v; }
        void SetYMin(i32 v) { i32 m = yMax(); y = v, height = m - v; }
        void SetXMax(i32 v) { width = v - x; }
        void SetYMax(i32 v) { height = v - y; }
        void SetPosition(Vector2Int v) { x = v.x, y = v.y; }
        void SetSize(Vector2Int v) { width = v.x, height = v.y; }
        void SetMin(Vector2Int v) { SetXMin(v.x), SetYMin(v.y); }
        void SetMax(Vector2Int v) { SetXMax(v.x), SetYMax(v.y); }
        void SetMinMax(Vector2Int minPosition, Vector2Int maxPosition) { SetMin(minPosition), SetMax(maxPosition); }
        void ClampToBounds(const RectInt& bounds);

        constexpr bool Contains(Vector2Int p) const { return p.x >= xMin() && p.y >= yMin() && p.x < xMax() && p.y < yMax(); }
        constexpr bool Overlaps(const RectInt& o) const
        {
            return o.xMin() < xMax() && o.xMax() > xMin() && o.yMin() < yMax() && o.yMax() > yMin();
        }
        constexpr bool Equals(const RectInt& o) const { return x == o.x && y == o.y && width == o.width && height == o.height; }
        constexpr bool operator==(const RectInt&) const = default;

        // Every position within, row by row (allPositionsWithin).
        struct PositionEnumerator
        {
            Vector2Int current;
            i32 xMin, xMax, yMax;
            constexpr Vector2Int operator*() const { return current; }
            constexpr PositionEnumerator& operator++()
            {
                if (++current.x >= xMax)
                    current.x = xMin, current.y++;
                return *this;
            }
            constexpr bool operator!=(const PositionEnumerator& o) const { return current != o.current; }
        };
        struct PositionCollection
        {
            PositionEnumerator first, last;
            constexpr PositionEnumerator begin() const { return first; }
            constexpr PositionEnumerator end() const { return last; }
        };
        constexpr PositionCollection allPositionsWithin() const
        {
            bool empty = xMin() >= xMax() || yMin() >= yMax();
            Vector2Int start{xMin(), yMin()};
            Vector2Int stop = empty ? start : Vector2Int{xMin(), yMax()};
            return {{start, xMin(), xMax(), yMax()}, {stop, xMin(), xMax(), yMax()}};
        }
    };

    inline constexpr RectInt RectInt::zero{0, 0, 0, 0};

    struct Ray;

    // center and extents (half the size) are the fields (C#: m_Center, m_Extents).
    HX_STRUCT(Namespace = "HelixEngine")
    struct Bounds
    {
        HX_FIELD(Name = "m_Center", Private) Vector3 center;
        HX_FIELD(Name = "m_Extents", Private) Vector3 extents;

        constexpr Bounds() = default;
        constexpr Bounds(Vector3 c, Vector3 size) : center(c), extents(size * 0.5f) {}

        constexpr Vector3 size() const { return extents * 2.0f; }
        constexpr Vector3 min() const { return center - extents; }
        constexpr Vector3 max() const { return center + extents; }
        void SetSize(Vector3 v) { extents = v * 0.5f; }
        void SetMin(Vector3 v) { SetMinMax(v, max()); }
        void SetMax(Vector3 v) { SetMinMax(min(), v); }
        void SetMinMax(Vector3 mn, Vector3 mx);
        void Encapsulate(Vector3 point);
        void Encapsulate(const Bounds& bounds);
        // The size grows by amount (each side by half of it).
        void Expand(f32 amount) { extents += Vector3(amount, amount, amount) * 0.5f; }
        void Expand(Vector3 amount) { extents += amount * 0.5f; }
        bool Contains(Vector3 point) const;
        bool Intersects(const Bounds& bounds) const;
        bool IntersectRay(const Ray& ray) const;
        bool IntersectRay(const Ray& ray, f32& distance) const;
        Vector3 ClosestPoint(Vector3 point) const;
        f32 SqrDistance(Vector3 point) const;
        constexpr bool Equals(const Bounds& o) const { return center.Equals(o.center) && extents.Equals(o.extents); }
        bool operator==(const Bounds& o) const { return center == o.center && extents == o.extents; }
    };

    // The direction is normalized (C#: m_Origin, m_Direction).
    HX_STRUCT(Namespace = "HelixEngine")
    struct Ray
    {
        HX_FIELD(Name = "m_Origin", Private) Vector3 origin;
        HX_FIELD(Name = "m_Direction", Private) Vector3 direction;

        Ray() = default;
        Ray(Vector3 o, Vector3 d) : origin(o), direction(Vector3::Normalize(d)) {}
        void SetDirection(Vector3 d) { direction = Vector3::Normalize(d); }
        constexpr Vector3 GetPoint(f32 distance) const { return origin + direction * distance; }
    };

    // The points p with Dot(normal, p) + distance == 0 (C#: m_Normal, m_Distance); three points clockwise seen from
    // the front give a normal toward the viewer.
    HX_STRUCT(Namespace = "HelixEngine")
    struct Plane
    {
        HX_FIELD(Name = "m_Normal", Private) Vector3 normal;
        HX_FIELD(Name = "m_Distance", Private) f32 distance = 0;

        Plane() = default;
        Plane(Vector3 inNormal, f32 d) : normal(Vector3::Normalize(inNormal)), distance(d) {}
        Plane(Vector3 inNormal, Vector3 inPoint) : normal(Vector3::Normalize(inNormal)), distance(-Vector3::Dot(normal, inPoint)) {}
        Plane(Vector3 a, Vector3 b, Vector3 c);

        Plane flipped() const { return Plane(-normal, -distance); }
        void Flip() { normal = -normal, distance = -distance; }
        void SetNormal(Vector3 n) { normal = Vector3::Normalize(n); }
        void SetNormalAndPosition(Vector3 inNormal, Vector3 inPoint) { *this = Plane(inNormal, inPoint); }
        void Set3Points(Vector3 a, Vector3 b, Vector3 c) { *this = Plane(a, b, c); }
        void Translate(Vector3 translation) { distance += Vector3::Dot(normal, translation); }
        static Plane Translate(Plane plane, Vector3 translation)
        {
            plane.Translate(translation);
            return plane;
        }

        f32 GetDistanceToPoint(Vector3 point) const { return Vector3::Dot(normal, point) + distance; }
        bool GetSide(Vector3 point) const { return GetDistanceToPoint(point) > 0.0f; }
        bool SameSide(Vector3 inPt0, Vector3 inPt1) const
        {
            f32 d0 = GetDistanceToPoint(inPt0), d1 = GetDistanceToPoint(inPt1);
            return (d0 > 0.0f && d1 > 0.0f) || (d0 <= 0.0f && d1 <= 0.0f);
        }
        Vector3 ClosestPointOnPlane(Vector3 point) const { return point - normal * GetDistanceToPoint(point); }
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
