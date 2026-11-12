#include <Helix/Math/Color.h>
#include <Helix/Math/Geometry.h>
#include <Helix/Math/Random.h>
#include <Helix/Core/Containers/Array.h>
#include <Helix/Jobs/Jobs.h>
#include <Helix/Testing/Test.h>

#include <cstring>

using namespace helix;

namespace
{
    bool Near(f32 a, f32 b, f32 eps = 1e-4f) { return mathf::Abs(a - b) <= eps; }
    bool Near(Vector3 a, Vector3 b, f32 eps = 1e-4f) { return Near(a.x, b.x, eps) && Near(a.y, b.y, eps) && Near(a.z, b.z, eps); }
}

HX_TEST(Mathf, Basics)
{
    HX_CHECK_EQ(mathf::Sign(0.0f), 1.0f);
    HX_CHECK_EQ(mathf::Round(2.5f), 2.0f);
    HX_CHECK_EQ(mathf::Round(3.5f), 4.0f);
    HX_CHECK_EQ(mathf::RoundToInt(-2.5f), -2);
    HX_CHECK_EQ(mathf::Lerp(0.0f, 10.0f, 2.0f), 10.0f);
    HX_CHECK_EQ(mathf::LerpUnclamped(0.0f, 10.0f, 2.0f), 20.0f);
    HX_CHECK_EQ(mathf::InverseLerp(5.0f, 5.0f, 3.0f), 0.0f);
    HX_CHECK(mathf::Approximately(1.0f, 1.0f + 1e-7f));
    HX_CHECK(!mathf::Approximately(1.0f, 1.001f));
    HX_CHECK(Near(mathf::Repeat(-1.0f, 3.0f), 2.0f));
    HX_CHECK(Near(mathf::PingPong(4.0f, 3.0f), 2.0f));
    HX_CHECK(Near(mathf::DeltaAngle(1080.0f, 90.0f), 90.0f));
    HX_CHECK(Near(mathf::DeltaAngle(10.0f, 350.0f), -20.0f));
    HX_CHECK(Near(mathf::MoveTowardsAngle(350.0f, 10.0f, 5.0f), 355.0f));
    HX_CHECK(Near(mathf::SmoothStep(0.0f, 1.0f, 0.5f), 0.5f));
    HX_CHECK_EQ(mathf::NextPowerOfTwo(129), 256);
    HX_CHECK_EQ(mathf::ClosestPowerOfTwo(100), 128);
    HX_CHECK(Near(mathf::GammaToLinearSpace(0.5f), 0.21404f));
    HX_CHECK(Near(mathf::LinearToGammaSpace(mathf::GammaToLinearSpace(0.7f)), 0.7f));
    f32 p = mathf::PerlinNoise(3.7f, 1.2f);
    HX_CHECK(p >= 0.0f && p <= 1.0f);
}

HX_TEST(Mathf, SmoothDampConverges)
{
    f32 v = 0.0f, x = 0.0f;
    for (int i = 0; i < 600; i++)
        x = mathf::SmoothDamp(x, 10.0f, v, 0.3f, mathf::Infinity, 1.0f / 60.0f);
    HX_CHECK(Near(x, 10.0f, 1e-3f));
    f32 v2 = 0.0f;
    f32 y = mathf::SmoothDamp(0.0f, 10.0f, v2, 0.3f, 1.0f, 1.0f / 60.0f);
    HX_CHECK(y <= 1.0f / 60.0f + 1e-4f);
}

HX_TEST(Vector3, Operations)
{
    Vector3 a(1, 2, 3), b(4, 5, 6);
    HX_CHECK_EQ(Vector3::Dot(a, b), 32.0f);
    HX_CHECK(Vector3::Cross(Vector3::right, Vector3::up) == Vector3::forward);
    HX_CHECK(Vector3::Normalize(Vector3(1e-6f, 0, 0)) == Vector3::zero);
    HX_CHECK(Near(Vector3::Angle(Vector3::right, Vector3::forward), 90.0f));
    HX_CHECK_EQ(Vector3::Angle(Vector3::zero, Vector3::forward), 0.0f);
    HX_CHECK(Near(Vector3::SignedAngle(Vector3::forward, Vector3::right, Vector3::up), 90.0f));
    HX_CHECK(Near(Vector3::Project(Vector3(1, 1, 0), Vector3::right), Vector3(1, 0, 0)));
    HX_CHECK(Near(Vector3::Reflect(Vector3(1, -1, 0), Vector3::up), Vector3(1, 1, 0)));
    HX_CHECK(Near(Vector3::MoveTowards(Vector3::zero, Vector3(10, 0, 0), 3.0f), Vector3(3, 0, 0)));
    HX_CHECK(Near(Vector3::ClampMagnitude(Vector3(10, 0, 0), 2.0f), Vector3(2, 0, 0)));
    HX_CHECK(Near(Vector3::Slerp(Vector3::right, Vector3::forward, 0.5f), Vector3(0.70710677f, 0, 0.70710677f)));
    HX_CHECK(Near(Vector3::Slerp(Vector3::right, Vector3::left, 0.5f).magnitude(), 1.0f));
    Vector3 n(0, 2, 0), t(1, 1, 0);
    Vector3::OrthoNormalize(n, t);
    HX_CHECK(Near(n, Vector3::up) && Near(t, Vector3::right));
    HX_CHECK(Vector3(1, 2, 3) == Vector3(1, 2, 3.000001f));
}

HX_TEST(Quaternion, EulerOrderAndAxes)
{
    HX_CHECK(Near(Quaternion::Euler(0, 90, 0) * Vector3::forward, Vector3::right));
    HX_CHECK(Near(Quaternion::Euler(90, 0, 0) * Vector3::forward, Vector3::down));
    HX_CHECK(Near(Quaternion::Euler(0, 0, 90) * Vector3::right, Vector3::up));
    Quaternion q = Quaternion::Euler(30, 60, 90);
    Quaternion zxy = Quaternion::Euler(0, 60, 0) * Quaternion::Euler(30, 0, 0) * Quaternion::Euler(0, 0, 90);
    HX_CHECK(q == zxy);
    HX_CHECK(Near(q.eulerAngles(), Vector3(30, 60, 90), 1e-3f));
    HX_CHECK(Near(Quaternion::Euler(-10, 370, 0).eulerAngles(), Vector3(350, 10, 0), 1e-3f));
    HX_CHECK(Quaternion::Euler(Quaternion::Euler(90, 45, 0).eulerAngles()) == Quaternion::Euler(90, 45, 0));
}

HX_TEST(Quaternion, ConstructionAndInterpolation)
{
    Quaternion look = Quaternion::LookRotation(Vector3::right);
    HX_CHECK(Near(look * Vector3::forward, Vector3::right));
    HX_CHECK(Near(look * Vector3::up, Vector3::up));
    HX_CHECK(Quaternion::LookRotation(Vector3::zero) == Quaternion::identity);
    Quaternion from = Quaternion::FromToRotation(Vector3::up, Vector3::forward);
    HX_CHECK(Near(from * Vector3::up, Vector3::forward));
    HX_CHECK(Near(Quaternion::FromToRotation(Vector3::up, Vector3::down) * Vector3::up, Vector3::down));
    Quaternion a = Quaternion::identity, b = Quaternion::Euler(0, 90, 0);
    HX_CHECK(Near(Quaternion::Angle(a, b), 90.0f, 1e-2f));
    HX_CHECK(Near(Quaternion::Angle(a, Quaternion::Slerp(a, b, 0.5f)), 45.0f, 1e-2f));
    HX_CHECK(Near(Quaternion::Angle(a, Quaternion::RotateTowards(a, b, 30.0f)), 30.0f, 1e-2f));
    HX_CHECK((Quaternion::Inverse(b) * b) == Quaternion::identity);
    f32 angle;
    Vector3 axis;
    Quaternion::AngleAxis(40.0f, Vector3(0, 0, 2)).ToAngleAxis(angle, axis);
    HX_CHECK(Near(angle, 40.0f, 1e-3f) && Near(axis, Vector3::forward));
}

HX_TEST(Matrix4x4, TransformsAndInverse)
{
    Quaternion r = Quaternion::Euler(10, 20, 30);
    Matrix4x4 m = Matrix4x4::TRS(Vector3(1, 2, 3), r, Vector3(2, 3, 4));
    Vector3 p(0.5f, -1.0f, 2.0f);
    Vector3 expected = r * Vector3::Scale(p, Vector3(2, 3, 4)) + Vector3(1, 2, 3);
    HX_CHECK(Near(m.MultiplyPoint3x4(p), expected));
    HX_CHECK(Near(m.MultiplyPoint(p), expected));
    HX_CHECK((m * m.inverse()).isIdentity());
    HX_CHECK(Near(m.determinant(), 24.0f, 1e-3f));
    HX_CHECK(Near(m.lossyScale(), Vector3(2, 3, 4)));
    HX_CHECK(m.rotation() == r);
    HX_CHECK(Near(m.GetPosition(), Vector3(1, 2, 3)));
    Matrix4x4 composed = Matrix4x4::Translate(Vector3(1, 2, 3)) * Matrix4x4::Rotate(r) * Matrix4x4::Scale(Vector3(2, 3, 4));
    HX_CHECK(composed == m);
    HX_CHECK(m.transpose().transpose() == m);
    HX_CHECK(Matrix4x4::zero.inverse() == Matrix4x4::zero);
}

HX_TEST(Matrix4x4, Projection)
{
    Matrix4x4 p = Matrix4x4::Perspective(60.0f, 16.0f / 9.0f, 0.3f, 1000.0f);
    Vector3 nearPoint = p.MultiplyPoint(Vector3(0, 0, -0.3f));
    Vector3 farPoint = p.MultiplyPoint(Vector3(0, 0, -1000.0f));
    HX_CHECK(Near(nearPoint.z, -1.0f) && Near(farPoint.z, 1.0f, 1e-3f));
    Matrix4x4 o = Matrix4x4::Ortho(-2, 2, -1, 1, 0.1f, 10.0f);
    HX_CHECK(Near(o.MultiplyPoint(Vector3(2, 1, -10)), Vector3(1, 1, 1)));
}

HX_TEST(Color, ConversionsAndHsv)
{
    Color32 c = Color(1.0f, 0.5f, 0.0f, 1.0f);
    HX_CHECK(c == Color32(255, 128, 0, 255));
    f32 h, s, v;
    Color::RGBToHSV(Color(0.2f, 0.4f, 0.8f), h, s, v);
    HX_CHECK(Color::HSVToRGB(h, s, v) == Color(0.2f, 0.4f, 0.8f));
    HX_CHECK(Near(Color(0.5f, 0.5f, 0.5f).linear().r, 0.21404f));
    HX_CHECK(Color::Lerp(Color::black, Color::white, 0.5f) == Color(0.5f, 0.5f, 0.5f, 1.0f));
}

HX_TEST(Geometry, BoundsRayPlane)
{
    Bounds b(Vector3::zero, Vector3(2, 2, 2));
    HX_CHECK(b.Contains(Vector3(1, 1, 1)) && !b.Contains(Vector3(1.1f, 0, 0)));
    b.Encapsulate(Vector3(3, 0, 0));
    HX_CHECK(Near(b.max(), Vector3(3, 1, 1)) && Near(b.min(), Vector3(-1, -1, -1)));
    f32 d;
    HX_CHECK(IntersectRay(b, Ray(Vector3(-5, 0, 0), Vector3::right), d) && Near(d, 4.0f));
    HX_CHECK(!IntersectRay(b, Ray(Vector3(-5, 5, 0), Vector3::right), d));
    Plane ground(Vector3::up, Vector3::zero);
    HX_CHECK(ground.Raycast(Ray(Vector3(0, 5, 0), Vector3::down), d) && Near(d, 5.0f));
    Rect r0{0, 0, 10, 10}, r1{5, 5, 10, 10};
    HX_CHECK(r0.Overlaps(r1));
    HX_CHECK(!r0.Contains(Vector2(10, 5)));
}

HX_TEST(Geometry, FrustumCullingScalarMatchesSimd)
{
    Matrix4x4 view = Matrix4x4::LookAt(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3::up).inverse();
    Matrix4x4 flip = Matrix4x4::Scale(Vector3(1, 1, -1));
    Matrix4x4 vp = Matrix4x4::Perspective(60.0f, 1.0f, 0.1f, 100.0f) * flip * view;
    Plane planes[6];
    CalculateFrustumPlanes(vp, planes);
    HX_CHECK(TestPlanesAABB(planes, Bounds(Vector3(0, 0, 10), Vector3::one)));
    HX_CHECK(!TestPlanesAABB(planes, Bounds(Vector3(0, 0, -10), Vector3::one)));
    HX_CHECK(!TestPlanesAABB(planes, Bounds(Vector3(0, 0, 200), Vector3::one)));

    constexpr usize n = 1003;
    static f32 cx[n], cy[n], cz[n], ex[n], ey[n], ez[n];
    static u8 visible[n];
    RandomGenerator rng(7);
    for (usize i = 0; i < n; i++)
    {
        cx[i] = rng.Range(-60.0f, 60.0f);
        cy[i] = rng.Range(-60.0f, 60.0f);
        cz[i] = rng.Range(-120.0f, 120.0f);
        ex[i] = ey[i] = ez[i] = rng.Range(0.1f, 3.0f);
    }
    CullBoundsSoA(planes, {cx, cy, cz, ex, ey, ez, n}, visible);
    usize mismatches = 0, seen = 0;
    for (usize i = 0; i < n; i++)
    {
        Bounds bi;
        bi.center = {cx[i], cy[i], cz[i]};
        bi.extents = {ex[i], ey[i], ez[i]};
        mismatches += TestPlanesAABB(planes, bi) != (visible[i] != 0) ? 1 : 0;
        seen += visible[i];
    }
    HX_CHECK_EQ(mismatches, 0u);
    HX_CHECK(seen > 0 && seen < n);
}

HX_TEST(Random, RangesAndDeterminism)
{
    RandomGenerator a(42), b(42);
    for (int i = 0; i < 100; i++)
        HX_CHECK_EQ(a.NextU32(), b.NextU32());
    RandomGenerator::State s = a.GetState();
    f32 v1 = a.value();
    a.SetState(s);
    HX_CHECK_EQ(a.value(), v1);
    bool sawMin = false;
    for (int i = 0; i < 10000; i++)
    {
        i32 r = a.Range(3, 7);
        HX_CHECK(r >= 3 && r < 7);
        sawMin |= r == 3;
        f32 f = a.Range(-1.0f, 1.0f);
        HX_CHECK(f >= -1.0f && f <= 1.0f);
        HX_CHECK(a.insideUnitSphere().sqrMagnitude() <= 1.0001f);
        HX_CHECK(Near(a.onUnitSphere().magnitude(), 1.0f, 1e-3f));
    }
    HX_CHECK(sawMin);
    HX_CHECK_EQ(a.Range(5, 5), 5);
    i32 rev = a.Range(10, 0);
    HX_CHECK(rev <= 10 && rev > 0);
}

HX_TEST(Geometry, TransformPointsMatchesScalar)
{
    Matrix4x4 m = Matrix4x4::TRS(Vector3(4, 5, 6), Quaternion::Euler(15, 25, 35), Vector3(1, 2, 3));
    Vector3 points[37], out[37];
    for (int i = 0; i < 37; i++)
        points[i] = Vector3(f32(i), f32(i * 2) - 3.0f, 0.5f * f32(i));
    TransformPoints(m, Span<const Vector3>(points, 37), Span<Vector3>(out, 37));
    for (int i = 0; i < 37; i++)
        HX_CHECK(Near(out[i], m.MultiplyPoint3x4(points[i]), 1e-3f));
}

HX_TEST(Geometry, ParallelCullAndTransformMatchSerial)
{
    constexpr usize n = 50003;
    Array<f32> soa;
    soa.Resize(n * 6);
    RandomGenerator rng(77);
    for (usize i = 0; i < n * 3; i++)
        soa[i] = rng.Range(-300.0f, 300.0f);
    for (usize i = n * 3; i < n * 6; i++)
        soa[i] = rng.Range(0.5f, 6.0f);
    BoundsSoA bounds{soa.Data(), soa.Data() + n, soa.Data() + 2 * n, soa.Data() + 3 * n, soa.Data() + 4 * n, soa.Data() + 5 * n, n};
    Plane planes[6];
    Matrix4x4 vp = Matrix4x4::Perspective(60.0f, 1.777f, 0.3f, 500.0f) * Matrix4x4::Scale(Vector3(1, 1, -1));
    CalculateFrustumPlanes(vp, planes);
    Array<u8> serial, parallel;
    serial.Resize(n);
    parallel.Resize(n);
    Array<Vector3> points, outSerial, outParallel;
    points.Resize(n);
    outSerial.Resize(n);
    outParallel.Resize(n);
    for (usize i = 0; i < n; i++)
        points[i] = Vector3(soa[i], soa[n + i], soa[2 * n + i]);
    CullBoundsSoA(planes, bounds, serial.Data());
    TransformPoints(vp, points, outSerial);

    for (int pass = 0; pass < 2; pass++)
    {
        if (pass == 1)
            jobs::Initialize();
        std::memset(parallel.Data(), 2, n);
        CullBoundsSoAParallel(planes, bounds, parallel.Data(), 1000);
        TransformPointsParallel(vp, points, outParallel, 1000);
        HX_CHECK(std::memcmp(serial.Data(), parallel.Data(), n) == 0);
        HX_CHECK(std::memcmp(outSerial.Data(), outParallel.Data(), n * sizeof(Vector3)) == 0);
    }
    usize visible = 0;
    for (u8 v : serial)
        visible += v;
    HX_CHECK(visible > 100 && visible < n);
    jobs::Shutdown();
}

HX_TEST(Math, RectIntContainsAndOverlaps)
{
    RectInt r{10, 20, 30, 40};
    HX_CHECK_EQ(r.xMax(), 40);
    HX_CHECK_EQ(r.yMax(), 60);
    HX_CHECK(r.Contains(Vector2Int{10, 20}));
    HX_CHECK(!r.Contains(Vector2Int{40, 20}));
    HX_CHECK(r.Overlaps(RectInt{39, 59, 5, 5}));
    HX_CHECK(!r.Overlaps(RectInt{40, 20, 5, 5}));
    HX_CHECK((r.size() == Vector2Int{30, 40}));
}
