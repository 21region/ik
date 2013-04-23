#ifndef CAMERA_H
#define CAMERA_H

#include "math/Matrix4D.h"

namespace ST
{
    class Camera
    {
    public:
        Camera(size_t width = 800, size_t height = 600);

        const Math::Matrix4D& GetView() const;
        const Math::Matrix4D& GetProjection() const;
    private:
        Math::Matrix4D view;
        Math::Matrix4D projection;
    };
}

#endif // CAMERA_H
