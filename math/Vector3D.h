#ifndef VECTOR3D_H_INCLUDED
#define VECTOR3D_H_INCLUDED

#include <ostream>
#include "Vector2D.h"

namespace Math
{
    class Vector3D
    {
    public:
        Vector3D() { m[0] = 0; m[1] = 0; m[2] = 0; }
        Vector3D(float x, float y, float z) { m[0] = x; m[1] = y; m[2] = z; }
        Vector3D(const Vector2D& v, float z) {
            m[0] = v[0]; m[1] = v[1]; m[2] = z;
        }
        explicit Vector3D(float a) { m[0] = a; m[1] = a; m[2] = a; }

        // Invert sign.
        Vector3D operator- () const { return Vector3D(-m[0], -m[1], -m[2]); }

        // Math operators.
        Vector3D operator+ (const Vector3D&) const;
        Vector3D operator- (const Vector3D&) const;
        Vector3D operator* (float) const;

        Vector3D& operator += (const Vector3D&);
        Vector3D& operator -= (const Vector3D&);

        // Access operators.
        float& operator[] (size_t index);
        const float& operator[] (size_t index) const;

        void Normalize();
        float Length() const;
        float LengthSquared() const;
        float Dot(const Vector3D&) const;
        Vector3D Cross(const Vector3D&) const;

    public:
        // Cross product computes a vector, perpendicular to both 'a' and 'b'.
        // The direction of result vector is determined by the right-hand rule.
        // The length of result vector is computed this way: |a|*|b|*sin(theta).
        static Vector3D Cross(const Vector3D&, const Vector3D&);
        // Make the length of a given vector equal 1.
        static Vector3D Normalize(const Vector3D&);
        // Linear interpolation.
        static Vector3D Lerp(const Vector3D&, const Vector3D&, float);

    private:
        friend std::ostream& operator<< (std::ostream&, const Vector3D&);
        friend std::ifstream& operator>> (std::ifstream&, Vector3D&);
        float m[3];
    };
}

#endif // VECTOR3D_H_INCLUDED
