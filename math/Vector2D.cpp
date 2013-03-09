#include "Vector2D.h"

namespace Math
{
    //-------------- Add rhs vector to this. --------------//
    Vector2D Vector2D::operator+ (const Vector2D& rhs) const
    {
        return Vector2D(m[0] + rhs.m[0], m[1] + rhs.m[1]);
    }
    //-------------- Subtract rhs vector from this. --------------//
    Vector2D Vector2D::operator- (const Vector2D& rhs) const
    {
        return Vector2D(m[0] - rhs.m[0], m[1] - rhs.m[1]);
    }
    //-------------- Multiply by scalar on the right. --------------//
    Vector2D Vector2D::operator* (float scalar) const
    {
        return Vector2D(m[0] * scalar, m[1] * scalar);
    }

    //-------------- Access element of vector. --------------//
    float& Vector2D::operator[] (size_t index)
    {
        assert(index < 2);
        return m[index];
    }
    //-------------- Get the value of 'index' element. --------------//
    float Vector2D::operator[] (size_t index) const
    {
        assert(index < 2);
        return m[index];
    }

    //-------------- Vector's length. --------------//
    float Vector2D::Length() const
    {
        return sqrt(m[0]*m[0] + m[1]*m[1]);
    }
    //-------------- Vector's squared length. --------------//
    float Vector2D::LengthSquared() const
    {
        return m[0]*m[0] + m[1]*m[1];
    }
    //-------------- Dot product. --------------//
    float Vector2D::Dot(const Vector2D& rhs) const
    {
        return m[0]*rhs.m[0] + m[1]*rhs.m[1];
    }

    /* Global functions. */

    //-------------- Multiply by scalar on the left. --------------//
    Vector2D operator* (float scalar, const Vector2D& rhs)
    {
        return Vector2D(scalar * rhs[0], scalar * rhs[1]);
    }

    //-------------- Ouput vector coordinates. --------------//
    std::ostream& operator<< (std::ostream& os, const Vector2D& rhs)
    {
        os << rhs[0] << ", " << rhs[1];
        return os;
    }
}
