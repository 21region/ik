#ifndef MATRIX4D_H_INCLUDED
#define MATRIX4D_H_INCLUDED

#include "Vector4D.h"

namespace Math
{
    /* Matrix4D is interpreted as column-major matrix.
     * That's why you multiply a matrix by a vector.
     */
    class Matrix4D
    {
    public:
        Matrix4D();
        Matrix4D(const Vector4D&, const Vector4D&, const Vector4D&, const Vector4D&);
        Matrix4D(float, float, float, float,
            float, float, float, float,
            float, float, float, float,
            float, float, float, float);

        explicit Matrix4D(float a)
        {
            m[0] = m[1] = m[2] = m[3] = a;
            m[4] = m[5] = m[6] = m[7] = a;
            m[8] = m[9] = m[10] = m[11] = a;
            m[12] = m[13] = m[14] = m[15] = a;
        }

        //Access operators.
        float& operator[] (size_t index);
        const float& operator[] (size_t index) const;

        float Det() const;
        void Transpose();
        void MakeIdentity();
        void RotX(float radian);
        void RotY(float radian);
        void RotZ(float radian);
        void Translate(float, float, float);

        //Math operators.
        Matrix4D operator+ (const Matrix4D& rhs) const;
        Matrix4D operator- (const Matrix4D& rhs) const;
        Matrix4D operator* (const Matrix4D& rhs) const;
        Vector4D operator* (const Vector4D& rhs) const;

        // Static methods.
        static Matrix4D Identity();
        static Matrix4D ProjectionMatrix(float, float, float, float);
        static Matrix4D MakeRotX(float radian);
        static Matrix4D MakeRotY(float radian);
        static Matrix4D MakeRotZ(float radian);
        static Matrix4D MakeTranslate(float, float, float);
        static Matrix4D Inverse(const Matrix4D& m);
    private:
        friend std::ostream& operator<< (std::ostream&, const Matrix4D&);
        float m[16];
    };
}

#endif // MATRIX4D_H_INCLUDED

