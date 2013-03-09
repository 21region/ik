#ifndef MATRIX2D_H_INCLUDED
#define MATRIX2D_H_INCLUDED

#include "Vector2D.h"

namespace Math
{
    /* Matrix2D is interpreted as column-major matrix.
     * That's why you multiply a matrix by a vector.
     */
    class Matrix2D
    {
    public:
        Matrix2D() {}
        Matrix2D(const Vector2D& row0, const Vector2D& row1);
        Matrix2D(float m00, float m01, float m10, float m11);

        // Access operators.
        float& operator[] (size_t index);
        float  operator[] (size_t index) const;

        void MakeIdentity();
        void MakeRotation (float radian);

        // Math operators.
        Matrix2D operator+ (const Matrix2D& rhs) const;
        Matrix2D operator- (const Matrix2D& rhs) const;
        Matrix2D operator* (const Matrix2D& rhs) const;
        Vector2D operator* (const Vector2D& rhs) const;
    private:
        float m[4];
    };
}

#endif // MATRIX2D_H_INCLUDED
