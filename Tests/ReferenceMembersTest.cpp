#include <Helix/Math/Color.h>
#include <Helix/Math/Geometry.h>
#include <Helix/Math/Random.h>
#include <Helix/Testing/Test.h>

#include <type_traits>

using namespace helix;

namespace
{
    bool Near(f32 a, f32 b, f32 e = 1e-4f) { return mathf::Abs(a - b) < e; }
}

HX_TEST(ReferenceMembers, StaticConstantsAndTrivialCopies)
{
    static_assert(Vector3::up.y == 1 && Vector3::back.z == -1 && Vector2::left.x == -1 && Vector4::one.w == 1);
    static_assert(Quaternion::identity.w == 1 && Matrix4x4::identity.m33 == 1 && Matrix4x4::zero.m00 == 0);
    static_assert(Vector2Int::up.y == 1 && Vector3Int::forward.z == 1 && Rect::zero.width == 0 && RectInt::zero.height == 0);
    static_assert(std::is_trivially_copyable_v<Vector3> && std::is_trivially_copyable_v<Matrix4x4> && std::is_trivially_copyable_v<Color>);
    static_assert(sizeof(Vector3) == 12 && sizeof(Matrix4x4) == 64 && sizeof(Bounds) == 24 && sizeof(Plane) == 16 && sizeof(Color32) == 4);
    HX_CHECK(Vector3::positiveInfinity.x == mathf::Infinity && Vector2::negativeInfinity.y == mathf::NegativeInfinity);
    HX_CHECK(Color::red.Equals(Color(1, 0, 0, 1)) && Color::clear.a == 0 && Near(Color::aliceBlue.r, 0.9411765f) && Color::gray5 == Color::gray);
}

HX_TEST(ReferenceMembers, VectorsInstanceMembersAndIntegers)
{
    Vector3 v(3, 0, 4);
    v.Normalize();
    HX_CHECK(Near(v.magnitude(), 1) && Near(v.x, 0.6f));
    v.Set(1, 2, 3);
    v.Scale(Vector3(2, 2, 2));
    HX_CHECK(v.Equals(Vector3(2, 4, 6)) && !v.Equals(Vector3(2, 4, 6.000001f)) && v == Vector3(2, 4, 6.000001f));
    Vector3 n(0, 2, 0), t(1, 1, 0), b(0, 0, 3);
    Vector3::OrthoNormalize(n, t, b);
    HX_CHECK(Near(Vector3::Dot(n, t), 0) && Near(Vector3::Dot(n, b), 0) && Near(Vector3::Dot(t, b), 0) && Near(b.magnitude(), 1));
    HX_CHECK(Vector3::SlerpUnclamped(Vector3::right, Vector3::up, 2.0f) == Vector3::left);
    HX_CHECK(Vector4::MoveTowards(Vector4::zero, Vector4(3, 4, 0, 0), 1) == Vector4(0.6f, 0.8f, 0, 0));
    Vector2Int a(3, -2);
    a.Clamp(Vector2Int(0, 0), Vector2Int(2, 2));
    HX_CHECK(a == Vector2Int(2, 0) && Vector2Int(3, 4).sqrMagnitude() == 25 && Near(Vector2Int(3, 4).magnitude(), 5));
    HX_CHECK(Vector2Int::RoundToInt(Vector2(1.5f, 2.5f)) == Vector2Int(2, 2) && Vector2Int::FloorToInt(Vector2(-0.5f, 0.5f)) == Vector2Int(-1, 0));
    Vector2 widened = Vector2Int(1, 2);
    HX_CHECK(widened == Vector2(1, 2) && Vector3Int(Vector2Int(1, 2)) == Vector3Int(1, 2, 0) && Vector2Int(Vector3Int(4, 5, 6)) == Vector2Int(4, 5));
    HX_CHECK(Vector3Int(1, 2, 3) * 2 == Vector3Int(2, 4, 6) && Vector3Int(4, 6, 8) / 2 == Vector3Int(2, 3, 4) && -Vector3Int::one == Vector3Int(-1, -1, -1));
    Vector2 velocity;
    Vector2 damped = Vector2::SmoothDamp(Vector2::zero, Vector2(10, 0), velocity, 0.3f, mathf::Infinity, 0.02f);
    HX_CHECK(damped.x > 0 && damped.x < 10 && velocity.x > 0);
}

HX_TEST(ReferenceMembers, RectsBoundsAndPlanes)
{
    Rect r(0, 0, 10, 5);
    r.SetXMin(2);
    HX_CHECK(r.x == 2 && r.width == 8 && r.xMax() == 10);
    r.SetCenter(Vector2(0, 0));
    HX_CHECK(Near(r.x, -4) && Near(r.y, -2.5f));
    Rect inverse(10, 10, -5, -5);
    HX_CHECK(!inverse.Contains(Vector3(7, 7, 0)) && inverse.Contains(Vector3(7, 7, 0), true));
    HX_CHECK(Rect(0, 0, 4, 4).Overlaps(Rect(6, 6, -4, -4), true) && !Rect(0, 0, 4, 4).Overlaps(Rect(6, 6, -4, -4)));
    HX_CHECK(Rect::NormalizedToPoint(Rect(0, 0, 4, 2), Vector2(0.5f, 0.5f)) == Vector2(2, 1));
    HX_CHECK(Rect::PointToNormalized(Rect(0, 0, 4, 2), Vector2(4, 4)) == Vector2(1, 1));

    RectInt cells(1, 1, 3, 2);
    i32 count = 0;
    Vector2Int last;
    for (Vector2Int p : cells.allPositionsWithin())
        count++, last = p;
    HX_CHECK(count == 6 && last == Vector2Int(3, 2) && cells.Contains(Vector2Int(3, 2)) && !cells.Contains(Vector2Int(4, 2)));
    RectInt clamped(-2, -2, 10, 10);
    clamped.ClampToBounds(RectInt(0, 0, 4, 4));
    HX_CHECK(clamped == RectInt(0, 0, 4, 4));

    Bounds box(Vector3::zero, Vector3(2, 2, 2));
    box.Expand(Vector3(2, 0, 0));
    HX_CHECK(box.size() == Vector3(4, 2, 2));
    f32 distance = 0;
    HX_CHECK(box.IntersectRay(Ray(Vector3(-10, 0, 0), Vector3::right), distance) && Near(distance, 8) && !box.IntersectRay(Ray(Vector3(0, 5, 0), Vector3::up)));

    Plane ground(Vector3::up, Vector3::zero);
    HX_CHECK(ground.SameSide(Vector3(0, 1, 0), Vector3(5, 2, 1)) && !ground.SameSide(Vector3(0, 1, 0), Vector3(0, -1, 0)));
    Plane lifted = Plane::Translate(ground, Vector3(0, -2, 0));
    HX_CHECK(Near(lifted.GetDistanceToPoint(Vector3(0, 2, 0)), 0));
    ground.Flip();
    HX_CHECK(ground.normal == Vector3::down && ground.flipped().normal == Vector3::up);
    Plane moved = Matrix4x4::Translate(Vector3(0, 3, 0)).TransformPlane(Plane(Vector3::up, Vector3::zero));
    HX_CHECK(Near(moved.GetDistanceToPoint(Vector3(7, 3, 1)), 0) && moved.normal == Vector3::up);
}

HX_TEST(ReferenceMembers, MatrixMembers)
{
    Matrix4x4 trs = Matrix4x4::TRS(Vector3(1, 2, 3), Quaternion::Euler(10, 20, 30), Vector3(2, 2, 2));
    HX_CHECK(trs.ValidTRS() && !Matrix4x4::Perspective(60, 1, 0.1f, 100).ValidTRS());
    Matrix4x4 inv;
    HX_CHECK(Matrix4x4::Inverse3DAffine(trs, inv) && inv == trs.inverse() && !Matrix4x4::Inverse3DAffine(Matrix4x4::zero, inv));
    Matrix4x4 m;
    m.SetTRS(Vector3(1, 0, 0), Quaternion::identity, Vector3::one);
    HX_CHECK(m[12] == 1 && m(0, 3) == 1 && m.isIdentity() == false && Matrix4x4::identity.isIdentity());
    FrustumPlanes fp = Matrix4x4::Frustum(-1, 2, -3, 4, 0.5f, 50).decomposeProjection();
    HX_CHECK(Near(fp.left, -1) && Near(fp.right, 2) && Near(fp.bottom, -3) && Near(fp.top, 4) && Near(fp.zNear, 0.5f) && Near(fp.zFar, 50, 1e-2f));
    FrustumPlanes op = Matrix4x4::Ortho(-4, 4, -2, 2, 1, 9).decomposeProjection();
    HX_CHECK(Near(op.left, -4) && Near(op.top, 2) && Near(op.zNear, 1) && Near(op.zFar, 9));
}

HX_TEST(ReferenceMembers, QuaternionMembers)
{
    Quaternion q;
    q.SetFromToRotation(Vector3::forward, Vector3::right);
    HX_CHECK(q * Vector3::forward == Vector3::right);
    q.SetLookRotation(Vector3::up, Vector3::back);
    HX_CHECK(q * Vector3::forward == Vector3::up);
    q.SetEulerAngles(Vector3(0, 90, 0));
    HX_CHECK(q == Quaternion::Euler(0, 90, 0) && Near(q.eulerAngles().y, 90, 1e-3f));
    q.Set(0, 0, 0, 2);
    q.Normalize();
    HX_CHECK(q.Equals(Quaternion::identity));
}

HX_TEST(ReferenceMembers, MathfAdditions)
{
    HX_CHECK(mathf::Abs(-3) == 3 && mathf::Min({3.0f, -1.0f, 2.0f}) == -1.0f && mathf::Max({1, 7, 4}) == 7 && mathf::Max<f32>({}) == 0);
    const f32 values[] = {0.0f, 1.0f, -2.5f, 65504.0f, 0.000061035156f, 0.0000000596046f, 1.0f / 3.0f};
    for (f32 v : values)
        HX_CHECK(Near(mathf::HalfToFloat(mathf::FloatToHalf(v)), v, mathf::Abs(v) * 1e-3f + 1e-8f));
    HX_CHECK(mathf::FloatToHalf(1.0f) == 0x3C00 && mathf::FloatToHalf(100000.0f) == 0x7C00 && mathf::HalfToFloat(0xC000) == -2.0f);
    for (f32 x = -10; x < 10; x += 0.37f)
    {
        f32 n = mathf::PerlinNoise1D(x);
        HX_CHECK(n >= 0 && n <= 1);
    }
    Color warm = mathf::CorrelatedColorTemperatureToRGB(2000), cold = mathf::CorrelatedColorTemperatureToRGB(15000);
    HX_CHECK(warm.r == 1 && warm.b < warm.g && cold.b == 1 && cold.r < 1);
}

HX_TEST(ReferenceMembers, TheStaticRandom)
{
    Random::InitState(1234);
    f32 first = Random::value();
    i32 dice = Random::Range(1, 7);
    Random::State saved = Random::state();
    f32 next = Random::value();
    Random::InitState(1234);
    HX_CHECK(Random::value() == first && Random::Range(1, 7) == dice);
    Random::SetState(saved);
    HX_CHECK(Random::value() == next);
    for (int i = 0; i < 2000; i++)
    {
        i32 swapped = Random::Range(10, 0);
        HX_CHECK(swapped >= 1 && swapped <= 10);
        HX_CHECK(Random::Range(5, 5) == 5);
        f32 f = Random::Range(2.0f, -2.0f);
        HX_CHECK(f >= -2 && f <= 2);
        HX_CHECK(Near(Random::onUnitCircle().magnitude(), 1) && Random::insideUnitCircle().magnitude() <= 1.0001f);
        Color c = Random::ColorHSV(0, 1, 1, 1, 1, 1, 0.5f, 0.5f);
        HX_CHECK(c.maxColorComponent() <= 1.0001f && c.a == 0.5f);
    }
}
