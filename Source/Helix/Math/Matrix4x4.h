#pragma once

#include <Helix/Math/Quaternion.h>

namespace helix
{
    // Column-major storage, column vectors: m<row><col>, memory order m00 m10 m20 m30 m01 ...
    struct Matrix4x4
    {
        f32 m00 = 0, m10 = 0, m20 = 0, m30 = 0;
        f32 m01 = 0, m11 = 0, m21 = 0, m31 = 0;
        f32 m02 = 0, m12 = 0, m22 = 0, m32 = 0;
        f32 m03 = 0, m13 = 0, m23 = 0, m33 = 0;

        constexpr Matrix4x4() = default;
        constexpr Matrix4x4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3)
            : m00(c0.x), m10(c0.y), m20(c0.z), m30(c0.w), m01(c1.x), m11(c1.y), m21(c1.z), m31(c1.w),
              m02(c2.x), m12(c2.y), m22(c2.z), m32(c2.w), m03(c3.x), m13(c3.y), m23(c3.z), m33(c3.w)
        {
        }

        static constexpr Matrix4x4 zero() { return {}; }
        static constexpr Matrix4x4 identity() { return {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; }

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
        bool isIdentity() const { return *this == identity(); }
        Vector3 GetPosition() const { return {m03, m13, m23}; }
        Vector3 lossyScale() const;
        Quaternion rotation() const;

        static Matrix4x4 Translate(Vector3 v);
        static Matrix4x4 Rotate(Quaternion q);
        static Matrix4x4 Scale(Vector3 v);
        static Matrix4x4 TRS(Vector3 position, Quaternion rotation, Vector3 scale);
        static Matrix4x4 Perspective(f32 fovDegrees, f32 aspect, f32 zNear, f32 zFar);
        static Matrix4x4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
        static Matrix4x4 Frustum(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
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
}
