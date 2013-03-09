#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

#include <cmath>
#include <cassert>
#include <ostream>

namespace Math
{
    class Vector2D
    {
    public:
        Vector2D() { m[0] = 0; m[1] = 0; }
        Vector2D(float x, float y) { m[0] = x; m[1] = y; }
        explicit Vector2D(float a) { m[0] = a; m[1] = a; }

        // Invert sign.
        Vector2D operator- () const {return Vector2D(-m[0], -m[1]); }

        // Math operators.
        Vector2D operator+ (const Vector2D&) const;
        Vector2D operator- (const Vector2D&) const;
        Vector2D operator* (float) const;

        // Access operators.
        float& operator[] (size_t index);
        float  operator[] (size_t index) const;

        float Length() const;
        float LengthSquared() const;
        float Dot(const Vector2D& rhs) const;

    private:
        float m[2];
    };
}

#endif // VECTOR2D_H_INCLUDED
