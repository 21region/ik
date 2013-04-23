#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <vector>
#include "OpenGL.h"
#include "Shader.h"
#include "Camera.h"
#include "math/Vector3D.h"

namespace ST
{
    class Graphics
    {
    public:
        Graphics();
        virtual ~Graphics();

        void DrawScene();
        void SetCamera(const Camera* camera);
        void LoadModel(const std::vector<Math::Vector3D>&,
                       const std::vector<Math::Vector3D>&,
                       const std::vector<size_t>&);

    protected:
        void create_context();
        void init_shaders();
        void init_opengl();
        void init_light() const;

        void errorThrow(std::string);

    protected:
        static const size_t width = 800;
        static const size_t height = 600;

        HDC       hdc;          // Handle to Device Context.
        HGLRC     hglrc;        // OpenGL handle.
        Shader    shader;       // Processing shader.

        bool loaded;
        size_t num_indices;
        GLuint    vao; // OpenGL buffers for single model.
        GLuint    vbo[3]; // If there will be more than 1 model
        // then there is a need to do this: graphics->GetDrawableModel(model);
        // GetDrawableModel() should wrap model so that it can be rendered.

        GLint viewLocation;
        Math::Matrix4D viewTrans;
        Math::Matrix4D invProj;
        Math::Matrix4D invView;
        const Camera* camera;
    };
}

#endif // GRAPHICS_H_INCLUDED
