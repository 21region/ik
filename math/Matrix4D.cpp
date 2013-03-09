#include <iomanip>
#include <iostream>
#include "Matrix4D.h"
#include "Matrix3D.h"
#include "Utility.h"

using namespace std;

namespace Math
{
    //-------------- By default the matrix is identity. --------------//
    Matrix4D::Matrix4D()
    {
        this->MakeIdentity();
    }
    //-------------- Init matrix row by row. --------------//
    Matrix4D::Matrix4D(const Vector4D& row0, const Vector4D& row1,
        const Vector4D& row2, const Vector4D& row3)
    {
        m[0] = row0[0]; m[1] = row0[1]; m[2] = row0[2]; m[3] = row0[3];
        m[4] = row1[0]; m[5] = row1[1]; m[6] = row1[2]; m[7] = row0[3];
        m[8] = row2[0]; m[10] = row2[1]; m[10] = row2[2]; m[11] = row0[3];
        m[12] = row2[0]; m[13] = row2[1]; m[14] = row2[2]; m[15] = row0[3];
    }
    //-------------- Init matrix by elements. --------------//
    Matrix4D::Matrix4D(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33)
    {
        m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
        m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
        m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }
    float Matrix4D::Det() const
    {
        float A, B, C, D;
        Matrix3D tmp;
        tmp[0] = m[5]; tmp[1] = m[6]; tmp[2] = m[7];
        tmp[3] = m[9]; tmp[4] = m[10]; tmp[5] = m[11];
        tmp[6] = m[13]; tmp[7] = m[14]; tmp[8] = m[15];
        A = tmp.Det();
        tmp[0] = m[4]; tmp[1] = m[6]; tmp[2] = m[7];
        tmp[3] = m[8]; tmp[4] = m[10]; tmp[5] = m[11];
        tmp[6] = m[12]; tmp[7] = m[14]; tmp[8] = m[15];
        B = tmp.Det();
        tmp[0] = m[4]; tmp[1] = m[5]; tmp[2] = m[7];
        tmp[3] = m[8]; tmp[4] = m[9]; tmp[5] = m[11];
        tmp[6] = m[12]; tmp[7] = m[13]; tmp[8] = m[15];
        C = tmp.Det();
        tmp[0] = m[4]; tmp[1] = m[5]; tmp[2] = m[6];
        tmp[3] = m[8]; tmp[4] = m[9]; tmp[5] = m[10];
        tmp[6] = m[12]; tmp[7] = m[13]; tmp[8] = m[14];
        D = tmp.Det();
        return m[0] * A - m[1] * B + m[2] * C - m[3] * D;
    }

    //-------------- Access element of matrix. --------------//
    float& Matrix4D::operator[] (size_t index)
    {
        assert(index < 16);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    const float& Matrix4D::operator[] (size_t index) const
    {
        assert(index < 16);
        return m[index];
    }

    //-------------- Transpose matrix. --------------//
    void Matrix4D::Transpose()
    {
        float temp;
        temp = m[1]; m[1] = m[4]; m[4] = temp;
        temp = m[2]; m[2] = m[8]; m[8] = temp;
        temp = m[6]; m[6] = m[9]; m[9] = temp;
        temp = m[3]; m[3] = m[12]; m[12] = temp;
        temp = m[7]; m[7] = m[13]; m[13] = temp;
        temp = m[11]; m[11] = m[14]; m[14] = temp;
    }
    //-------------- Make this matrix identity. --------------//
    void Matrix4D::MakeIdentity()
    {
        m[0] = 1; m[4] = 0; m[8] = 0; m[12] = 0;
        m[1] = 0; m[5] = 1; m[9] = 0; m[13] = 0;
        m[2] = 0; m[6] = 0; m[10] = 1; m[14] = 0;
        m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix4D::RotX (float radian)
    {
        this->MakeIdentity();
        m[5] = cos(radian) ; m[6] = sin(radian);
        m[9] = -sin(radian); m[10] = cos(radian);
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix4D::RotY (float radian)
    {
        this->MakeIdentity();
        m[0] = cos(radian);m[2] = -sin(radian);
        m[8] = sin(radian); m[10] = cos(radian);
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix4D::RotZ (float radian)
    {
        this->MakeIdentity();
        m[0] = cos(radian) ; m[1] = sin(radian);
        m[4] = -sin(radian); m[5] = cos(radian);
    }
    //---------- Make translation matrix. ----------//
    void Matrix4D::Translate(float dx, float dy, float dz)
    {
        this->MakeIdentity();
        m[12] = dx; m[13] = dy; m[14] = dz;
    }

    //-------------- Add two matrices. --------------//
    Matrix4D Matrix4D::operator+ (const Matrix4D& rhs) const
    {
        Matrix4D res(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3],
                     m[4] + rhs[4], m[5] + rhs[5], m[6] + rhs[6], m[7] + rhs[7],
                     m[8] + rhs[8], m[9] + rhs[9], m[10] + rhs[10], m[11] + rhs[11],
                     m[12] + rhs[12], m[13] + rhs[13], m[14] + rhs[14], m[15] + rhs[15]);
        return res;
    }
    //-------------- Subtract two matrices. --------------//
    Matrix4D Matrix4D::operator- (const Matrix4D& rhs) const
    {
        Matrix4D res(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3],
                     m[4] - rhs[4], m[5] - rhs[5], m[6] - rhs[6], m[7] - rhs[7],
                     m[8] - rhs[8], m[9] - rhs[9], m[10] - rhs[10], m[11] - rhs[11],
                     m[12] - rhs[12], m[13] - rhs[13], m[14] - rhs[14], m[15] - rhs[15]);
        return res;
    }
    //-------------- Multiply two matrices. --------------//
    Matrix4D Matrix4D::operator* (const Matrix4D& rhs) const
    {
        Matrix4D res(0);

        for (int i = 0; i < 4; i++)     // column
            for (int j = 0; j < 4; j++) // row
                for (int k = 0; k < 4; k++)
                    res[j + i * 4] += m[j + k * 4] * rhs[k + i * 4];

        return res;
    }
    //-------------- Multiply this matrix by vector. --------------//
    Vector4D Matrix4D::operator* (const Vector4D& rhs) const{
        Vector4D res(m[0] * rhs[0] + m[4] * rhs[1] + m[8] * rhs[2] + m[12] * rhs[3],
                     m[1] * rhs[0] + m[5] * rhs[1] + m[9] * rhs[2] + m[13] * rhs[3],
                     m[2] * rhs[0] + m[6] * rhs[1] + m[10] * rhs[2] + m[14] * rhs[3],
                     m[3] * rhs[0] + m[7] * rhs[1] + m[11] * rhs[2] + m[15] * rhs[3]);
        return res;
    }

    //---------- Construct column-major projection matrix. ----------//
    Matrix4D Matrix4D::ProjectionMatrix (float fovy, float aspect, float zNear, float zFar)
    {
        Matrix4D proj(0);
        float f = 1 / tan( fovy * DEG_2_RAD / 2 );
        proj[0] = f / aspect;
        proj[5] = f;
        proj[10] = -(zFar + zNear) / (zFar - zNear);
        proj[14] = -2 * zFar * zNear / (zFar - zNear);
        proj[11] = -1;

        return proj;
    }

    //---------- Make ccw rotation matrix around X axis. ----------//
    Matrix4D Matrix4D::MakeRotX (float radian)
    {
        Matrix4D rot;
        rot.MakeIdentity();
        rot.m[5] = cos(radian) ; rot.m[6] = sin(radian);
        rot.m[9] = -sin(radian); rot.m[10] = cos(radian);
        return rot;
    }
    //---------- Make ccw rotation matrix around Y axis. ----------//
    Matrix4D Matrix4D::MakeRotY (float radian)
    {
        Matrix4D rot;
        rot.MakeIdentity();
        rot.m[0] = cos(radian); rot.m[2] = -sin(radian);
        rot.m[8] = sin(radian); rot.m[10] = cos(radian);
        return rot;
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    Matrix4D Matrix4D::MakeRotZ (float radian)
    {
        Matrix4D rot;
        rot.MakeIdentity();
        rot.m[0] = cos(radian) ; rot.m[1] = sin(radian);
        rot.m[4] = -sin(radian); rot.m[5] = cos(radian);
        return rot;
    }
    //---------- Make translation matrix. ----------//
    Matrix4D Matrix4D::MakeTranslate(float dx, float dy, float dz)
    {
        Matrix4D trans;
        trans.MakeIdentity();
        trans.m[12] = dx; trans.m[13] = dy; trans.m[14] = dz;
        return trans;
    }
    //---------- Make identity matrix. ----------//
    Matrix4D Matrix4D::Identity()
    {
        Matrix4D identity;
        identity.MakeIdentity();
        return identity;
    }

    //---------- Return inverted matrix mat. ----------//
    Matrix4D Matrix4D::Inverse(const Matrix4D& mat)
    {
        const float det = 1.0f / mat.Det();
        if (det == 0)
            return mat;

        #define MDET3(a0,a1,a2,a3,a4,a5,a6,a7,a8) \
                    ( a0 * (a4 * a8 - a5 * a7) \
                    - a1 * (a3 * a8 - a5 * a6) \
                    + a2 * (a3 * a7 - a4 * a6) )

        // ~ 160 mul & 80 add
        return Matrix4D(
             MDET3(mat[5], mat[6], mat[7], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]) * det,
            -MDET3(mat[1], mat[2], mat[3], mat[9], mat[10], mat[11], mat[13], mat[14], mat[15]) * det,
             MDET3(mat[1], mat[2], mat[3], mat[5], mat[ 6], mat[ 7], mat[13], mat[14], mat[15]) * det,
            -MDET3(mat[1], mat[2], mat[3], mat[5], mat[ 6], mat[ 7], mat[ 9], mat[10], mat[11]) * det,
            -MDET3(mat[4], mat[6], mat[7], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]) * det,
             MDET3(mat[0], mat[2], mat[3], mat[8], mat[10], mat[11], mat[12], mat[14], mat[15]) * det,
            -MDET3(mat[0], mat[2], mat[3], mat[4], mat[ 6], mat[ 7], mat[12], mat[14], mat[15]) * det,
             MDET3(mat[0], mat[2], mat[3], mat[4], mat[ 6], mat[ 7], mat[ 8], mat[10], mat[11]) * det,
             MDET3(mat[4], mat[5], mat[7], mat[8], mat[ 9], mat[11], mat[12], mat[13], mat[15]) * det,
            -MDET3(mat[0], mat[1], mat[3], mat[8], mat[ 9], mat[11], mat[12], mat[13], mat[15]) * det,
             MDET3(mat[0], mat[1], mat[3], mat[4], mat[ 5], mat[ 7], mat[12], mat[13], mat[15]) * det,
            -MDET3(mat[0], mat[1], mat[3], mat[4], mat[ 5], mat[ 7], mat[ 8], mat[ 9], mat[11]) * det,
            -MDET3(mat[4], mat[5], mat[6], mat[8], mat[ 9], mat[10], mat[12], mat[13], mat[14]) * det,
             MDET3(mat[0], mat[1], mat[2], mat[8], mat[ 9], mat[10], mat[12], mat[13], mat[14]) * det,
            -MDET3(mat[0], mat[1], mat[2], mat[4], mat[ 5], mat[ 6], mat[12], mat[13], mat[14]) * det,
             MDET3(mat[0], mat[1], mat[2], mat[4], mat[ 5], mat[ 6], mat[ 8], mat[ 9], mat[10]) * det);

        #undef MDET3
    }

    //-------------- Ouput matrix elements. --------------//
    std::ostream& operator<< (std::ostream& os, const Matrix4D& rhs)
    {
        os << fixed << setprecision(3)
           << rhs[0] << ' ' << rhs[4] << ' ' << rhs[8] << ' ' << rhs[12] << '\n'
           << rhs[1] << ' ' << rhs[5] << ' ' << rhs[9] << ' ' << rhs[13] << '\n'
           << rhs[2] << ' ' << rhs[6] << ' ' << rhs[10] << ' ' << rhs[14] << '\n'
           << rhs[3] << ' ' << rhs[7] << ' ' << rhs[11] << ' ' << rhs[15] << '\n';
        return os;
    }
}

