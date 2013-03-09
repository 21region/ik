#ifndef MATRIX3D_H_INCLUDED
#define MATRIX3D_H_INCLUDED

#include "Vector3D.h"
#include "Matrix4D.h"

namespace Math
{
    /* Matrix3D is interpreted as column-major matrix.
     * That's why you multiply a matrix by a vector.
     */
    class Matrix3D
    {
    public:
        friend class Matrix4D;

        Matrix3D() {}
        Matrix3D(const Vector3D&, const Vector3D&, const Vector3D&);
        Matrix3D(float, float, float,
                 float, float, float,
                 float, float, float);
        explicit Matrix3D(float a)
        {
            m[0] = a; m[1] = a; m[2] = a;
            m[3] = a; m[4] = a; m[5] = a;
            m[6] = a; m[7] = a; m[8] = a;
        }

        float Det() const;

        // Access operators.
        float& operator[] (size_t index);
        float  operator[] (size_t index) const;

        void MakeIdentity();
        void RotX(float radian);
        void RotY(float radian);
        void RotZ(float radian);

        // Math operators.
        Matrix3D operator+ (const Matrix3D& rhs) const;
        Matrix3D operator- (const Matrix3D& rhs) const;
        Matrix3D operator* (const Matrix3D& rhs) const;
        Vector3D operator* (const Vector3D& rhs) const;
    private:
        friend std::ostream& operator<< (std::ostream&, const Matrix3D&);
        float m[9];
    };
}

#endif // MATRIX3D_H_INCLUDED
