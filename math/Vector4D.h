#ifndef VECTOR4D_H_INCLUDED
#define VECTOR4D_H_INCLUDED

#include <cmath>
#include <cassert>
#include <ostream>

#include "Vector2D.h"
#include "Vector3D.h"

namespace Math
{
    class Vector4D
    {
    public:
        Vector4D() { m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 0; }
        Vector4D(float x, float y, float z, float w) {
            m[0] = x; m[1] = y; m[2] = z; m[3] = w;
        }
        Vector4D(const Vector3D& v, float w) {
            m[0] = v[0]; m[1] = v[1]; m[2] = v[2]; m[3] = w;
        }
        Vector4D(const Vector2D& v, float z, float w) {
            m[0] = v[0]; m[1] = v[1]; m[2] = z; m[3] = w;
        }
        explicit Vector4D(float a) { m[0] = a; m[1] = a; m[2] = a; m[3] = a; }

        // Invert sign.
        Vector4D operator- () const {
            return Vector4D(-m[0], -m[1], -m[2], -m[3]);
        }

        // Math operators.
        Vector4D operator+ (const Vector4D&) const;
        Vector4D operator- (const Vector4D&) const;
        Vector4D operator* (float) const;

        // Access operators.
        float& operator[] (size_t index);
        float  operator[] (size_t index) const;

        float Length() const;
        float LengthSquared() const;
        float Dot(const Vector4D&) const;

    private:
        friend std::ostream& operator<< (std::ostream&, const Vector4D&);
        float m[4];
    };
}

#endif // VECTOR4D_H_INCLUDED
