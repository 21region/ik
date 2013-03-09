#include "Matrix2D.h"

namespace Math
{
    //-------------- Init matrix row by row. --------------//
    Matrix2D::Matrix2D(const Vector2D& row0, const Vector2D& row1)
    {
        m[0] = row0[0]; m[1] = row0[1];
        m[2] = row1[0]; m[3] = row1[1];
    }
    //-------------- Init matrix by elements. --------------//
    Matrix2D::Matrix2D(float m00, float m01, float m10, float m11)
    {
        m[0] = m00; m[1] = m01;
        m[2] = m10; m[3] = m11;
    }

    //-------------- Access element of matrix. --------------//
    float& Matrix2D::operator[] (size_t index)
    {
        assert(index < 4);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    float Matrix2D::operator[] (size_t index) const
    {
        assert(index < 4);
        return m[index];
    }

    //-------------- Make this matrix identity. --------------//
    void Matrix2D::MakeIdentity()
    {
        m[0] = 1.0f; m[1] = 0.0f;
        m[2] = 0.0f; m[3] = 1.0f;
    }
    //-------------- Make ccw rotation matrix. --------------//
    void Matrix2D::MakeRotation (float radian)
    {
        m[0] = cos(radian); m[1] = -sin(radian);
        m[2] = sin(radian); m[3] =  cos(radian);
    }

    //-------------- Add two matrices. --------------//
    Matrix2D Matrix2D::operator+ (const Matrix2D& rhs) const
    {
        Matrix2D res(m[0] + rhs[0], m[1] + rhs[1],
                     m[2] + rhs[2], m[3] + rhs[3]);
        return res;
    }
    //-------------- Subtract two matrices. --------------//
    Matrix2D Matrix2D::operator- (const Matrix2D& rhs) const
    {
        Matrix2D res(m[0] - rhs[0], m[1] - rhs[1],
                     m[2] - rhs[2], m[3] - rhs[3]);
        return res;
    }
    //-------------- Multiply two matrices. --------------//
    Matrix2D Matrix2D::operator* (const Matrix2D& rhs) const
    {
        Matrix2D res;
        res[0] = m[0] * rhs[0] + m[1] * rhs[2]; // 0 row, 0 column
        res[1] = m[0] * rhs[1] + m[1] * rhs[3]; // 0 row, 1 column
        res[2] = m[2] * rhs[0] + m[3] * rhs[2]; // 1 row, 0 column
        res[3] = m[2] * rhs[1] + m[3] * rhs[3]; // 1 row, 1 column

        return res;
    }
    //-------------- Multiply this matrix by vector. --------------//
    Vector2D Matrix2D::operator* (const Vector2D& rhs) const
    {
        Vector2D res(m[0] * rhs[0] + m[1] * rhs[1],
                     m[2] * rhs[0] + m[3] * rhs[1]);
        return res;
    }


    //-------------- Ouput matrix elements. --------------//
    std::ostream& operator<< (std::ostream& os, const Matrix2D& rhs)
    {
        os << '\n';
        os << rhs[0] << ' ' << rhs[1] << '\n'
           << rhs[2] << ' ' << rhs[3];
        return os;
    }
}
