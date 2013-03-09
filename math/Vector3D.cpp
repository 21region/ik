#include <cmath>
#include <fstream>
#include <cassert>
#include "Vector3D.h"

namespace Math
{
    //-------------- Add rhs vector to this. --------------//
    Vector3D Vector3D::operator+ (const Vector3D& rhs) const
    {
        return Vector3D(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2]);
    }
    //-------------- Subtract rhs vector from this. --------------//
    Vector3D Vector3D::operator- (const Vector3D& rhs) const
    {
        return Vector3D(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2]);
    }
    //-------------- Multiply by scalar on the right. --------------//
    Vector3D Vector3D::operator* (float scalar) const
    {
        return Vector3D(m[0] * scalar, m[1] * scalar, m[2] * scalar);
    }

    //-------------- Add rhs vector to this. --------------//
    Vector3D& Vector3D::operator += (const Vector3D& rhs)
    {
        m[0] += rhs[0];
        m[1] += rhs[1];
        m[2] += rhs[2];
        return *this;
    }
    //-------------- Subtract rhs vector from this. --------------//
    Vector3D& Vector3D::operator -= (const Vector3D& rhs)
    {
        m[0] -= rhs[0];
        m[1] -= rhs[1];
        m[2] -= rhs[2];
        return *this;
    }

    //-------------- Access element of vector. --------------//
    float& Vector3D::operator[] (size_t index)
    {
        assert(index < 3);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    const float& Vector3D::operator[] (size_t index) const
    {
        assert(index < 3);
        return m[index];
    }

    //----- Make the length of a given vector equal 1 -----//
    void Vector3D::Normalize()
    {
        float length = this->Length();
        m[0] /= length;
        m[1] /= length;
        m[2] /= length;
    }

    //-------------- Vector's length. --------------//
    float Vector3D::Length() const
    {
        return sqrt(m[0]*m[0] + m[1]*m[1] + m[2]*m[2]);
    }
    //-------------- Vector's squared length. --------------//
    float Vector3D::LengthSquared() const
    {
        return m[0]*m[0] + m[1]*m[1] + m[2]*m[2];
    }
    //-------------- Dot product. --------------//
    float Vector3D::Dot(const Vector3D& rhs) const
    {
        return m[0]*rhs[0] + m[1]*rhs[1] + m[2]*rhs[2];
    }
    //-------------- Cross product --------------//
    Vector3D Vector3D::Cross(const Vector3D& rhs) const
    {
        return Cross(*this, rhs);
    }

    /* Static functions */

    //----- Compute cross product of 'a' and 'b' (a x b) -----//
    Vector3D Vector3D::Cross(const Vector3D& a, const Vector3D& b)
    {
        return Vector3D(a[1]*b[2] - a[2]*b[1],
                        a[2]*b[0] - a[0]*b[2],
                        a[0]*b[1] - a[1]*b[0]);
    }

    //----- Make the length of a given vector equal 1 -----//
    Vector3D Vector3D::Normalize(const Vector3D& vec)
    {
        float length = vec.Length();
        return Vector3D(vec[0] / length, vec[1] / length, vec[2] / length);
    }

    //---- Interpolate between a and b by the quotient ----//
    Vector3D Vector3D::Lerp(const Vector3D& a, const Vector3D& b, float quotient)
    {
        return Vector3D(a[0] + quotient * (b[0] - a[0]),
                        a[1] + quotient * (b[1] - a[1]),
                        a[2] + quotient * (b[2] - a[2]));
    }

    /* Global functions. */

    //-------------- Multiply by scalar on the left. --------------//
    Vector3D operator* (float scalar, const Vector3D& rhs)
    {
        return Vector3D(scalar * rhs[0], scalar * rhs[1], scalar * rhs[2]);
    }

    //-------------- Ouput vector coordinates. --------------//
    std::ostream& operator<< (std::ostream& os, const Vector3D& rhs)
    {
        os << rhs[0] << ", " << rhs[1] << ", " << rhs[2];
        return os;
    }

    //-------------- Simple input. --------------//
    std::ifstream& operator>> (std::ifstream& in, Vector3D& rhs)
    {
        in >> rhs[0] >> rhs[1] >> rhs[2];
        return in;
    }
}
