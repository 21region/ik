#include "Vector4D.h"

namespace Math
{
    //-------------- Add rhs vector to this. --------------//
    Vector4D Vector4D::operator+ (const Vector4D& rhs) const
    {
        return Vector4D(m[0] + rhs[0],
                        m[1] + rhs[1],
                        m[2] + rhs[2],
                        m[3] + rhs[3]);
    }
    //-------------- Subtract rhs vector from this. --------------//
    Vector4D Vector4D::operator- (const Vector4D& rhs) const
    {
        return Vector4D(m[0] - rhs[0],
                        m[1] - rhs[1],
                        m[2] - rhs[2],
                        m[3] - rhs[3]);
    }
    //-------------- Multiply by scalar on the right. --------------//
    Vector4D Vector4D::operator* (float scalar) const
    {
        return Vector4D(m[0] * scalar,
                        m[1] * scalar,
                        m[2] * scalar,
                        m[3] * scalar);
    }

    //-------------- Access element of vector. --------------//
    float& Vector4D::operator[] (size_t index)
    {
        assert(index < 4);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    float Vector4D::operator[] (size_t index) const
    {
        assert(index < 4);
        return m[index];
    }

    //-------------- Vector's length. --------------//
    float Vector4D::Length() const
    {
        return sqrt(m[0]*m[0] + m[1]*m[1] + m[2]*m[2] + m[3]*m[3]);
    }
    //-------------- Vector's squared length. --------------//
    float Vector4D::LengthSquared() const
    {
        return m[0]*m[0] + m[1]*m[1] + m[2]*m[2] + m[3]*m[3];
    }
    //-------------- Dot product. --------------//
    float Vector4D::Dot(const Vector4D& rhs) const
    {
        return m[0]*rhs[0] + m[1]*rhs[1] + m[2]*rhs[2] + m[3]*m[3];
    }

    /* Global functions. */

    //-------------- Multiply by scalar on the left. --------------//
    Vector4D operator* (float scalar, const Vector4D& rhs)
    {
        return Vector4D(scalar * rhs[0],
                        scalar * rhs[1],
                        scalar * rhs[2],
                        scalar * rhs[3]);
    }

    //-------------- Ouput vector coordinates. --------------//
    std::ostream& operator<< (std::ostream& os, const Vector4D& rhs)
    {
        os << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << ", " << rhs[3];
        return os;
    }
}
