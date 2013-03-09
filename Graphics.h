#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "MD5Model.h"

namespace ST
{
    class Graphics
    {
    public:
        Graphics();
        virtual ~Graphics();

        void DrawScene();
        void AddModel(const std::string& fileName);
        void AddAnimation(const std::string& fileName);
        void Update(float deltaTimeSec);

        void Rotate(float angle);
        void AffectJoint();
        int GetJoint(size_t x, size_t y);

    protected:
        void createContext();
        void initShaders();
        void initOpenGL();

        void errorThrow(std::string);

    protected:
        static const size_t width = 800;
        static const size_t height = 600;

        HDC       hdc;          // Handle to Device Context.
        HGLRC     hglrc;        // OpenGL handle.
        Shader    shader;       // Processing shader.
        MD5Model* model;        // Rendered model.

        float angle;               // Should be in Camera class.
        GLint viewLocation;        //
        Math::Matrix4D viewTrans;  //
        Math::Matrix4D invProj;
        Math::Matrix4D invView;
    };
}

#endif // GRAPHICS_H_INCLUDED
