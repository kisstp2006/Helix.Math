#include <Helix/Math/Geometry.h>
#include <Helix/Math/Random.h>
#include <Helix/Testing/Bench.h>

using namespace helix;
using helix::testing::DoNotOptimize;

namespace
{
    constexpr usize kCount = 100000;
    f32 cx[kCount], cy[kCount], cz[kCount], ex[kCount], ey[kCount], ez[kCount];
    u8 visible[kCount];
    Plane planes[6];

    void Setup()
    {
        static bool done = false;
        if (done)
            return;
        done = true;
        Random rng(1);
        for (usize i = 0; i < kCount; i++)
        {
            cx[i] = rng.Range(-500.0f, 500.0f);
            cy[i] = rng.Range(-50.0f, 50.0f);
            cz[i] = rng.Range(-500.0f, 500.0f);
            ex[i] = ey[i] = ez[i] = rng.Range(0.5f, 4.0f);
        }
        Matrix4x4 vp = Matrix4x4::Perspective(60.0f, 1.777f, 0.3f, 1000.0f) * Matrix4x4::Scale(Vector3(1, 1, -1));
        CalculateFrustumPlanes(vp, planes);
    }
}

HX_BENCH(CullBoundsSoA_100k)
{
    Setup();
    state.SetItemsPerIteration(kCount);
    while (state.KeepRunning())
    {
        CullBoundsSoA(planes, {cx, cy, cz, ex, ey, ez, kCount}, visible);
        DoNotOptimize(visible[kCount / 2]);
    }
}

HX_BENCH(CullBoundsScalar_100k)
{
    Setup();
    state.SetItemsPerIteration(kCount);
    while (state.KeepRunning())
    {
        for (usize i = 0; i < kCount; i++)
        {
            Bounds b;
            b.center = {cx[i], cy[i], cz[i]};
            b.extents = {ex[i], ey[i], ez[i]};
            visible[i] = u8(TestPlanesAABB(planes, b));
        }
        DoNotOptimize(visible[kCount / 2]);
    }
}

HX_BENCH(MatrixMultiply)
{
    Matrix4x4 a = Matrix4x4::TRS(Vector3(1, 2, 3), Quaternion::Euler(10, 20, 30), Vector3::one());
    Matrix4x4 b = a.inverse();
    while (state.KeepRunning())
    {
        a = a * b;
        DoNotOptimize(a);
    }
}

HX_BENCH(QuaternionRotateVector)
{
    Quaternion q = Quaternion::Euler(10, 20, 30);
    Vector3 v(1, 2, 3);
    while (state.KeepRunning())
    {
        v = q * v;
        DoNotOptimize(v);
    }
}
