/*
Quaternion.h
=====
Description.
Quaternion is represented like this: q = [w (x y z)]
When we think about applying rotations, quaternion is the same thing as
3x3 matrix representing rotation. But the thing is that 3x3 matrix can repre
sent much more than that. If we need only rotation, we save extra bytes and
use quaternions.
It helpful to think that quaternion and matrix represent angular displacement
but euler angle represent orientation. The distinction is similar to the
difference between vector and point.
The reason why Euler angles are not used is Gimbal lock. That's where the 4th
component of a quaternion becomes helpful.

Let unit vector 'n' to be the axis of rotation. The positive rotation is defined by
the right-hand rule and equal to 'θ'. Then the pair (θ, n) defines angular
displacement as a rotation of 'θ' radians around unit vector 'n'. This pair is
stored in quaternion not directly, but in this form:
q = [cos(θ/2) sin(θ/2)*n]

That's why rotation quaternion is a unit quaternion: |q| = 1.

Author: Okhterov Egor Andreevich (longhorn4500@gmail.com)
*/

#ifndef QUATERNION_H_INCLUDED
#define QUATERNION_H_INCLUDED

#include <ostream>
#include "Vector3D.h"

namespace Math
{
    class Quaternion
    {
    public:
        Quaternion(float w = 0, float x = 0, float y = 0, float z = 0)
            : w(w), x(x), y(y), z(z)    { }
        Quaternion(float angle, const Vector3D& axis);

        Quaternion operator+ (const Quaternion& rhs) const;
        Quaternion operator- (const Quaternion& rhs) const;
        Quaternion operator* (const Quaternion& rhs) const;

        // Access operators.
        float& operator[] (size_t index);
        const float& operator[] (size_t index) const;

        float Norm() const;
        float Length() const;
        Vector3D Rotate(const Vector3D& v) const;
        Vector3D InverseRotate(const Vector3D& v) const;
        Quaternion Conjugate() const;

        void Normalize();
        void ComputeW();

        static float Dot(const Quaternion& a, const Quaternion& b);
        static Quaternion Slerp(const Quaternion& a, const Quaternion& b,
                                float quotient);

        friend std::ostream& operator<< (std::ostream&, const Quaternion&);
        friend std::ifstream& operator>> (std::ifstream&, Quaternion&);
        float w, x, y, z;
    };
}

#endif // QUATERNION_H_INCLUDED
