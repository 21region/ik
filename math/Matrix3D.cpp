#include "Matrix3D.h"

namespace Math
{
    //-------------- Init matrix row by row --------------//
    Matrix3D::Matrix3D(const Vector3D& row0,
                       const Vector3D& row1, const Vector3D& row2)
    {
        m[0] = row0[0]; m[1] = row0[1]; m[2] = row0[2];
        m[3] = row1[0]; m[4] = row1[1]; m[5] = row1[2];
        m[6] = row2[0]; m[7] = row2[1]; m[8] = row2[2];
    }
    //-------------- Init matrix column by column --------------//
    Matrix3D::Matrix3D(float m00, float m01, float m02,
                       float m10, float m11, float m12,
                       float m20, float m21, float m22)
    {
        m[0] = m00; m[1] = m01; m[2] = m02;
        m[3] = m10; m[4] = m11; m[5] = m12;
        m[6] = m20; m[7] = m21; m[8] = m22;
    }

    //-------------- Return matrix determinant --------------//
    float Matrix3D::Det() const
    {
        return m[0] * m[4] * m[8] + m[3] * m[7] * m[2] + m[6] * m[1] * m[5] -
               m[6] * m[4] * m[2] - m[7] * m[5] * m[0] - m[8] * m[3] * m[1];
    }

    //-------------- Access element of matrix. --------------//
    float& Matrix3D::operator[] (size_t index)
    {
        assert(index < 9);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    float Matrix3D::operator[] (size_t index) const
    {
        assert(index < 9);
        return m[index];
    }

    //-------------- Make this matrix identity. --------------//
    void Matrix3D::MakeIdentity()
    {
        m[0] = 1; m[1] = 0; m[2] = 0;
        m[3] = 0; m[4] = 1; m[5] = 0;
        m[6] = 0; m[7] = 0; m[8] = 1;
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix3D::RotX (float radian)
    {
        m[0] = 1; m[1] = 0          ; m[2] = 0;
        m[3] = 0; m[4] = cos(radian); m[5] = -sin(radian);
        m[6] = 0; m[7] = sin(radian); m[8] =  cos(radian);
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix3D::RotY (float radian)
    {
        m[0] =  cos(radian); m[1] = 0; m[2] = sin(radian);
        m[3] =  0          ; m[4] = 1; m[5] = 0;
        m[6] = -sin(radian); m[7] = 0; m[8] = cos(radian);
    }
    //---------- Make ccw rotation matrix around Z axis. ----------//
    void Matrix3D::RotZ (float radian)
    {
        m[0] = cos(radian); m[1] = -sin(radian); m[2] = 0;
        m[3] = sin(radian); m[4] =  cos(radian); m[5] = 0;
        m[6] = 0;           m[7] =  0;         ; m[8] = 1;
    }

    //-------------- Add two matrices. --------------//
    Matrix3D Matrix3D::operator+ (const Matrix3D& rhs) const
    {
        Matrix3D res(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2],
                     m[3] + rhs[3], m[4] + rhs[4], m[5] + rhs[5],
                     m[6] + rhs[6], m[7] + rhs[7], m[8] + rhs[8]);
        return res;
    }
    //-------------- Subtract two matrices. --------------//
    Matrix3D Matrix3D::operator- (const Matrix3D& rhs) const
    {
        Matrix3D res(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2],
                     m[3] - rhs[3], m[4] - rhs[4], m[5] - rhs[5],
                     m[6] - rhs[6], m[7] - rhs[7], m[8] - rhs[8]);
        return res;
    }
    //-------------- Multiply two matrices. --------------//
    Matrix3D Matrix3D::operator* (const Matrix3D& rhs) const
    {
        Matrix3D res(0);

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    res[j + i * 3] += m[k + i * 3] * rhs[j + k * 3];

        return res;
    }
    //-------------- Multiply this matrix by vector. --------------//
    Vector3D Matrix3D::operator* (const Vector3D& rhs) const
    {
        Vector3D res(m[0] * rhs[0] + m[1] * rhs[1] + m[2] * rhs[2],
                     m[3] * rhs[0] + m[4] * rhs[1] + m[5] * rhs[2],
                     m[6] * rhs[0] + m[7] * rhs[1] + m[8] * rhs[2]);
        return res;
    }


    //-------------- Ouput matrix elements. --------------//
    std::ostream& operator<< (std::ostream& os, const Matrix3D& rhs)
    {
        os << '\n';
        os << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2] << '\n'
           << rhs[3] << ' ' << rhs[4] << ' ' << rhs[5] << '\n'
           << rhs[6] << ' ' << rhs[7] << ' ' << rhs[8];
        return os;
    }
}
