#pragma once

#include <Helix/Math/Quaternion.h>

namespace helix
{
    struct Plane;

    // The planes of a projection's frustum at the near plane (Matrix4x4::Frustum, decomposeProjection).
    HX_STRUCT(Namespace = "HelixEngine")
    struct FrustumPlanes
    {
        HX_FIELD() f32 left = 0;
        HX_FIELD() f32 right = 0;
        HX_FIELD() f32 bottom = 0;
        HX_FIELD() f32 top = 0;
        HX_FIELD() f32 zNear = 0;
        HX_FIELD() f32 zFar = 0;
    };

    // Column-major storage, column vectors: m<row><col>, memory order m00 m10 m20 m30 m01 ... (index = row + column * 4).
    HX_STRUCT(Namespace = "HelixEngine")
    struct Matrix4x4
    {
        HX_FIELD() f32 m00 = 0;
        HX_FIELD() f32 m10 = 0;
        HX_FIELD() f32 m20 = 0;
        HX_FIELD() f32 m30 = 0;
        HX_FIELD() f32 m01 = 0;
        HX_FIELD() f32 m11 = 0;
        HX_FIELD() f32 m21 = 0;
        HX_FIELD() f32 m31 = 0;
        HX_FIELD() f32 m02 = 0;
        HX_FIELD() f32 m12 = 0;
        HX_FIELD() f32 m22 = 0;
        HX_FIELD() f32 m32 = 0;
        HX_FIELD() f32 m03 = 0;
        HX_FIELD() f32 m13 = 0;
        HX_FIELD() f32 m23 = 0;
        HX_FIELD() f32 m33 = 0;

        static const Matrix4x4 zero, identity;

        constexpr Matrix4x4() = default;
        constexpr Matrix4x4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3)
            : m00(c0.x), m10(c0.y), m20(c0.z), m30(c0.w), m01(c1.x), m11(c1.y), m21(c1.z), m31(c1.w),
              m02(c2.x), m12(c2.y), m22(c2.z), m32(c2.w), m03(c3.x), m13(c3.y), m23(c3.z), m33(c3.w)
        {
        }

        f32* Data() { return &m00; }
        const f32* Data() const { return &m00; }
        f32& operator[](int index) { return Data()[index]; }
        f32 operator[](int index) const { return Data()[index]; }
        f32& operator()(int row, int column) { return Data()[row + column * 4]; }
        f32 operator()(int row, int column) const { return Data()[row + column * 4]; }

        Vector4 GetColumn(int i) const { return {(*this)(0, i), (*this)(1, i), (*this)(2, i), (*this)(3, i)}; }
        Vector4 GetRow(int i) const { return {(*this)(i, 0), (*this)(i, 1), (*this)(i, 2), (*this)(i, 3)}; }
        void SetColumn(int i, Vector4 v);
        void SetRow(int i, Vector4 v);

        Matrix4x4 transpose() const;
        Matrix4x4 inverse() const;
        f32 determinant() const;
        bool isIdentity() const;
        Vector3 GetPosition() const { return {m03, m13, m23}; }
        Vector3 lossyScale() const;
        Quaternion rotation() const;
        FrustumPlanes decomposeProjection() const;
        bool ValidTRS() const;
        void SetTRS(Vector3 pos, Quaternion q, Vector3 s) { *this = TRS(pos, q, s); }
        Plane TransformPlane(const Plane& plane) const;

        static Matrix4x4 Translate(Vector3 v);
        static Matrix4x4 Rotate(Quaternion q);
        static Matrix4x4 Scale(Vector3 v);
        static Matrix4x4 TRS(Vector3 position, Quaternion rotation, Vector3 scale);
        static Matrix4x4 Perspective(f32 fovDegrees, f32 aspect, f32 zNear, f32 zFar);
        static Matrix4x4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
        static Matrix4x4 Frustum(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
        static Matrix4x4 Frustum(FrustumPlanes fp) { return Frustum(fp.left, fp.right, fp.bottom, fp.top, fp.zNear, fp.zFar); }
        // The inverse of an affine matrix (faster than inverse); false when it has none.
        static bool Inverse3DAffine(const Matrix4x4& input, Matrix4x4& result);
        static Matrix4x4 LookAt(Vector3 from, Vector3 to, Vector3 up);
        static Matrix4x4 Inverse(const Matrix4x4& m) { return m.inverse(); }
        static Matrix4x4 Transpose(const Matrix4x4& m) { return m.transpose(); }

        Matrix4x4 operator*(const Matrix4x4& b) const;
        Vector4 operator*(Vector4 v) const;
        Vector3 MultiplyPoint(Vector3 p) const;
        Vector3 MultiplyPoint3x4(Vector3 p) const;
        Vector3 MultiplyVector(Vector3 v) const;
        bool operator==(const Matrix4x4& o) const;
    };

    inline constexpr Matrix4x4 Matrix4x4::zero{};
    inline constexpr Matrix4x4 Matrix4x4::identity{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
}
