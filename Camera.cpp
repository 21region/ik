#include "Camera.h"

using namespace std;
using namespace Math;

namespace ST
{
    Camera::Camera(size_t width, size_t height)
    {
        view.MakeIdentity();
        projection = Matrix4D::ProjectionMatrix(
            45.0f, float(width) / float(height), 1, 1000);
    }

    const Matrix4D& Camera::GetView() const
    {
        return view;
    }

    const Matrix4D& Camera::GetProjection() const
    {
        return projection;
    }
}
