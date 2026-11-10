#include <Helix/Math/Matrix4x4.h>
#include <Helix/Math/Simd.h>

namespace helix
{
    using namespace mathf;

    void Matrix4x4::SetColumn(int i, Vector4 v)
    {
        (*this)(0, i) = v.x;
        (*this)(1, i) = v.y;
        (*this)(2, i) = v.z;
        (*this)(3, i) = v.w;
    }

    void Matrix4x4::SetRow(int i, Vector4 v)
    {
        (*this)(i, 0) = v.x;
        (*this)(i, 1) = v.y;
        (*this)(i, 2) = v.z;
        (*this)(i, 3) = v.w;
    }

    Matrix4x4 Matrix4x4::transpose() const
    {
        Matrix4x4 r;
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                r(row, col) = (*this)(col, row);
        return r;
    }

    f32 Matrix4x4::determinant() const
    {
        const f32* m = Data();
        f32 a = m[10] * m[15] - m[14] * m[11];
        f32 b = m[6] * m[15] - m[14] * m[7];
        f32 c = m[6] * m[11] - m[10] * m[7];
        f32 d = m[2] * m[15] - m[14] * m[3];
        f32 e = m[2] * m[11] - m[10] * m[3];
        f32 f = m[2] * m[7] - m[6] * m[3];
        return m[0] * (m[5] * a - m[9] * b + m[13] * c) - m[4] * (m[1] * a - m[9] * d + m[13] * e) +
               m[8] * (m[1] * b - m[5] * d + m[13] * f) - m[12] * (m[1] * c - m[5] * e + m[9] * f);
    }

    Matrix4x4 Matrix4x4::inverse() const
    {
        const f32* m = Data();
        f32 inv[16];
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        f32 det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0.0f)
            return zero();
        f32 s = 1.0f / det;
        Matrix4x4 r;
        for (int i = 0; i < 16; i++)
            r[i] = inv[i] * s;
        return r;
    }

    Vector3 Matrix4x4::lossyScale() const
    {
        return {Vector3(m00, m10, m20).magnitude(), Vector3(m01, m11, m21).magnitude(), Vector3(m02, m12, m22).magnitude()};
    }

    Quaternion Matrix4x4::rotation() const
    {
        Vector3 f(m02, m12, m22), u(m01, m11, m21);
        return Quaternion::LookRotation(f, u);
    }

    Matrix4x4 Matrix4x4::Translate(Vector3 v)
    {
        Matrix4x4 m = identity();
        m.m03 = v.x;
        m.m13 = v.y;
        m.m23 = v.z;
        return m;
    }

    Matrix4x4 Matrix4x4::Rotate(Quaternion q)
    {
        f32 x2 = q.x * 2, y2 = q.y * 2, z2 = q.z * 2;
        f32 xx = q.x * x2, yy = q.y * y2, zz = q.z * z2, xy = q.x * y2, xz = q.x * z2, yz = q.y * z2;
        f32 wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;
        Matrix4x4 m;
        m.m00 = 1 - (yy + zz);
        m.m10 = xy + wz;
        m.m20 = xz - wy;
        m.m01 = xy - wz;
        m.m11 = 1 - (xx + zz);
        m.m21 = yz + wx;
        m.m02 = xz + wy;
        m.m12 = yz - wx;
        m.m22 = 1 - (xx + yy);
        m.m33 = 1;
        return m;
    }

    Matrix4x4 Matrix4x4::Scale(Vector3 v)
    {
        Matrix4x4 m;
        m.m00 = v.x;
        m.m11 = v.y;
        m.m22 = v.z;
        m.m33 = 1;
        return m;
    }

    Matrix4x4 Matrix4x4::TRS(Vector3 p, Quaternion q, Vector3 s)
    {
        Matrix4x4 m = Rotate(q);
        m.m00 *= s.x;
        m.m10 *= s.x;
        m.m20 *= s.x;
        m.m01 *= s.y;
        m.m11 *= s.y;
        m.m21 *= s.y;
        m.m02 *= s.z;
        m.m12 *= s.z;
        m.m22 *= s.z;
        m.m03 = p.x;
        m.m13 = p.y;
        m.m23 = p.z;
        return m;
    }

    Matrix4x4 Matrix4x4::Perspective(f32 fovDegrees, f32 aspect, f32 zNear, f32 zFar)
    {
        f32 cot = 1.0f / Tan(fovDegrees * Deg2Rad * 0.5f);
        f32 depth = zNear - zFar;
        Matrix4x4 m;
        m.m00 = cot / aspect;
        m.m11 = cot;
        m.m22 = (zFar + zNear) / depth;
        m.m23 = 2.0f * zFar * zNear / depth;
        m.m32 = -1.0f;
        return m;
    }

    Matrix4x4 Matrix4x4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
    {
        Matrix4x4 m = identity();
        m.m00 = 2.0f / (right - left);
        m.m11 = 2.0f / (top - bottom);
        m.m22 = -2.0f / (zFar - zNear);
        m.m03 = -(right + left) / (right - left);
        m.m13 = -(top + bottom) / (top - bottom);
        m.m23 = -(zFar + zNear) / (zFar - zNear);
        return m;
    }

    Matrix4x4 Matrix4x4::Frustum(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
    {
        Matrix4x4 m;
        m.m00 = 2.0f * zNear / (right - left);
        m.m11 = 2.0f * zNear / (top - bottom);
        m.m02 = (right + left) / (right - left);
        m.m12 = (top + bottom) / (top - bottom);
        m.m22 = -(zFar + zNear) / (zFar - zNear);
        m.m23 = -2.0f * zFar * zNear / (zFar - zNear);
        m.m32 = -1.0f;
        return m;
    }

    Matrix4x4 Matrix4x4::LookAt(Vector3 from, Vector3 to, Vector3 up)
    {
        return TRS(from, Quaternion::LookRotation(to - from, up), Vector3::one());
    }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& b) const
    {
        Matrix4x4 r;
        simd::float4 c0 = simd::Load(&m00), c1 = simd::Load(&m01), c2 = simd::Load(&m02), c3 = simd::Load(&m03);
        for (int col = 0; col < 4; col++)
        {
            const f32* bc = b.Data() + col * 4;
            simd::float4 v = simd::Mul(c0, simd::Splat(bc[0]));
            v = simd::MulAdd(c1, simd::Splat(bc[1]), v);
            v = simd::MulAdd(c2, simd::Splat(bc[2]), v);
            v = simd::MulAdd(c3, simd::Splat(bc[3]), v);
            simd::Store(r.Data() + col * 4, v);
        }
        return r;
    }

    Vector4 Matrix4x4::operator*(Vector4 v) const
    {
        return {m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w, m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w,
                m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w, m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w};
    }

    Vector3 Matrix4x4::MultiplyPoint(Vector3 p) const
    {
        Vector4 r = *this * Vector4(p, 1.0f);
        f32 w = 1.0f / r.w;
        return {r.x * w, r.y * w, r.z * w};
    }

    Vector3 Matrix4x4::MultiplyPoint3x4(Vector3 p) const
    {
        return {m00 * p.x + m01 * p.y + m02 * p.z + m03, m10 * p.x + m11 * p.y + m12 * p.z + m13, m20 * p.x + m21 * p.y + m22 * p.z + m23};
    }

    Vector3 Matrix4x4::MultiplyVector(Vector3 v) const
    {
        return {m00 * v.x + m01 * v.y + m02 * v.z, m10 * v.x + m11 * v.y + m12 * v.z, m20 * v.x + m21 * v.y + m22 * v.z};
    }

    bool Matrix4x4::operator==(const Matrix4x4& o) const
    {
        for (int i = 0; i < 4; i++)
            if (!(GetColumn(i) == o.GetColumn(i)))
                return false;
        return true;
    }
}
