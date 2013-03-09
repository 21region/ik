#include <cmath>
#include <fstream>
#include "Quaternion.h"

namespace Math
{
    Quaternion::Quaternion(float angle, const Vector3D& axis)
    {
        angle /= 2;
        float sinHalfAlpha = sin(angle);
        float cosHalfAlpha = cos(angle);

        float length = sqrt(axis[0] * axis[0] +
                            axis[1] * axis[1] +
                            axis[2] * axis[2]);
        x = axis[0] * sinHalfAlpha / length;
        y = axis[1] * sinHalfAlpha / length;
        z = axis[2] * sinHalfAlpha / length;

        w = cosHalfAlpha;
    }

    Quaternion Quaternion::operator+ (const Quaternion& rhs) const
    {
        Quaternion res;
        res.w = w + rhs.w;
        res.x = x + rhs.x;
        res.y = y + rhs.y;
        res.z = z + rhs.z;

        return res;
    }

    Quaternion Quaternion::operator- (const Quaternion& rhs) const
    {
        Quaternion res;
        res.w = w - rhs.w;
        res.x = x - rhs.x;
        res.y = y - rhs.y;
        res.z = z - rhs.z;

        return res;
    }

    Quaternion Quaternion::operator* (const Quaternion& rhs) const
    {
        Quaternion res;
        res.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
        res.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
        res.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
        res.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

        return res;
    }

    //-------------- Access element of vector. --------------//
    float& Quaternion::operator[] (size_t index)
    {
        assert(index < 4);
        return *(&w + index);
    }
    //-------------- Get the value of 'index' element. --------------//
    const float& Quaternion::operator[] (size_t index) const
    {
        assert(index < 4);
        return *(&w + index);
    }

    float Quaternion::Norm() const
    {
        return w * w + x * x + y * y + z * z;
    }

    float Quaternion::Length() const
    {
        return sqrt(w * w + x * x + y * y + z * z);
    }

    Vector3D Quaternion::Rotate(const Vector3D& v) const
    {
        Quaternion q(0, v[0], v[1], v[2]);
        Quaternion res = *this * q * this->Conjugate();
        return Vector3D(res.x, res.y, res.z);
    }

    Vector3D Quaternion::InverseRotate(const Vector3D& v) const
    {
        Quaternion q(0, v[0], v[1], v[2]);
        Quaternion res = this->Conjugate() * q * (*this);
        return Vector3D(res.x, res.y, res.z);
    }

    Quaternion Quaternion::Conjugate() const
    {
        Quaternion q;
        q.w = w; q.x = -x; q.y = -y; q.z = -z;
        return q;
    }

    void Quaternion::Normalize()
    {
        float length = sqrt(w * w + x * x + y * y + z * z);

        if (length > 0.0f)
        {
            w /= length;
            x /= length;
            y /= length;
            z /= length;
        }
    }

    //--------- Compute w coordinate of unit quaternion ---------//
    void Quaternion::ComputeW()
    {
        float t = 1.0 - x * x - y * y - z * z;

        if (t < 0.0f)
            w = 0;
        else
            w = -sqrt(t);
    }

    float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
    {
        return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b,
                                 float quotient)
    {
        Quaternion result;
        if (quotient <= 0.0f)
        {
            result = a;
            return result;
        }
        else if (quotient >= 1.0f)
        {
            result = b;
            return result;
        }

        float bw = b.w, bx = b.x, by = b.y, bz = b.z;

        // If angle is obtuse, rotate quaternion so that it becomes acute.
        float cosOmega = Quaternion::Dot(a, b);
        if (cosOmega < 0.0f)
        {
            bw = -bw; bx = -bx; by = -by; bz = -bz;
            cosOmega = -cosOmega;
        }

        float k0, k1;
        if (cosOmega > 0.9999f)
        {
            // Perform linear interpolation, so we don't have divide by 0.
            k0 = 1.0 - quotient;
            k1 = quotient;
        }
        else
        {
            float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
            float omega = atan2(sinOmega, cosOmega);
            float oneOverSinOmega = 1.0f / sinOmega;
            k0 = sin((1.0f - quotient) * omega) * oneOverSinOmega;
            k1 = sin(quotient * omega) * oneOverSinOmega;
        }

        result.w = k0 * a.w + k1 * bw;
        result.x = k0 * a.x + k1 * bx;
        result.y = k0 * a.y + k1 * by;
        result.z = k0 * a.z + k1 * bz;

        return result;
    }

    //-------------- Ouput quaternion coordinates --------------//
    std::ostream& operator<< (std::ostream& os, const Quaternion& rhs)
    {
        os << rhs.w << ", " << rhs.x << ", " << rhs.y << ", " << rhs.z;
        return os;
    }

    //------------ Simple input. Expect only x, y, z ---------//
    std::ifstream& operator>> (std::ifstream& in, Quaternion& rhs)
    {
        in >> rhs.x >> rhs.y >> rhs.z;
        return in;
    }
}
