#include <sstream>
#include <stdexcept>
#include "Graphics.h"
#include "Window.h"
#include "math/Utility.h"
#include <iostream>

using namespace std;
using namespace Math;

namespace ST
{
    //-------------- Graphics constructor --------------//
    Graphics::Graphics() : model(0), angle(0)  // Should not be here.
    {
        createContext();
        initOpenGL();
        initShaders();

        OPENGL_CHECK_FOR_ERRORS();
    }

    //-------------- Graphics destructor --------------//
    Graphics::~Graphics()
    {
        if (model)
        {
            delete model;
            model = 0;
        }
    }

    //-------------- Init processing shaders --------------//
    void Graphics::initOpenGL()
    {
        glViewport(0, 0, 800, 600);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glDepthRange(0.0f, 1.0f);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
    }

    //-------------- Init processing shaders --------------//
    void Graphics::initShaders()
    {
        shader.CreateShader(GL_VERTEX_SHADER, "data/shaders/main.vert");
        shader.CreateShader(GL_FRAGMENT_SHADER, "data/shaders/main.frag");
        shader.CreateProgram();
        shader.Activate();

        // Set projection matrix.
        GLint projLocation = shader.GetUniformLocation("projection");
        Matrix4D projTrans =
            Matrix4D::ProjectionMatrix(45.0f, float(width) / height, 1, 1000);
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projTrans[0]);
        invProj = Matrix4D::Inverse(projTrans);
        invView = Matrix4D::Identity();

        // Set view matrix;
        viewLocation = shader.GetUniformLocation("view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewTrans[0]);

        // Set the light.
        GLint dirLocation = shader.GetUniformLocation("dirToLight");
        GLint colorLocation = shader.GetUniformLocation("lightColor");
        glUniform3f(dirLocation, 0, 0, 1);
        glUniform3f(colorLocation, 1, 1, 1);
    }

    //-------------- Rotate object --------------//
    void Graphics::Rotate(float angle)
    {
        this->angle += angle;

        if (this->angle >= 2 * PI)
            this->angle -= 2 * PI;
        else if (this->angle <= -2 * PI)
            this->angle += 2 * PI;

        //shader.SetUniformMatrix("view", Matrix4D::MakeRotY(-this->angle));
        if (model)
        {
            Matrix4D m = model->GetModelTrans() *
                         Matrix4D::MakeRotZ(this->angle);
            shader.SetUniformMatrix("model", m);
        }
    }

    //-------------- Test how joint can be changed --------------//
    void Graphics::AffectJoint()
    {
        if (model)
            model->AffectJoint();
    }

    //-------------- Returns index of the found joint --------------//
    int Graphics::GetJoint(size_t x, size_t y)
    {
        float x_clip = 2.0f * x / width - 1;
        float y_clip = 1 - 2.0f * y / height;

        Vector4D pick_ray = invProj * Vector4D(x_clip, y_clip, 0, 1);
        pick_ray[0] /= pick_ray[3];
        pick_ray[1] /= pick_ray[3];
        pick_ray[2] /= pick_ray[3];
        pick_ray[3] /= pick_ray[3];

        return -1;
    }

    //-------------- Update the scene --------------//
    void Graphics::Update(float deltaTimeSec)
    {
        if (model)
            model->Update(deltaTimeSec);
    }

    //-------------- Load and place model in a scene --------------//
    void Graphics::AddModel(const string& fileName)
    {
        if (model)
        {
            delete model;
            model = 0;
        }

        model = new MD5Model();
        model->Load(fileName, shader);
    }

    //-------------- Load animation for existing model --------------//
    void Graphics::AddAnimation(const std::string& fileName)
    {
        if (model)
            model->LoadAnim(fileName);
    }

    //-------------- Game logic --------------//
    void Graphics::DrawScene()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (model)
            model->Draw(true);

        SwapBuffers(hdc);

        OPENGL_CHECK_FOR_ERRORS();
    }

    //-------------- OpenGL context creation --------------//
    void Graphics::createContext()
    {
        // Get a handle to a display device context.
        hdc  = ::GetDC(MainWindow->GetHandle());

        // Describe pixel format.
        PIXELFORMATDESCRIPTOR pfd;
        ::memset(&pfd, 0, sizeof(pfd));
        pfd.nSize      = sizeof(pfd);
        pfd.nVersion   = 1;
        pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                         PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;

        // Request closest to above pixel format.
        int format = ::ChoosePixelFormat(hdc, &pfd);
        if (!format || !::SetPixelFormat(hdc, format, &pfd))
            errorThrow("Setting pixel format fail.");

        // Create temporary rendering context.
        // It is needed to get wglCreateContextAttribsARB function.
        HGLRC hRCTemp = ::wglCreateContext(hdc);
        if (!hRCTemp || !::wglMakeCurrent(hdc, hRCTemp))
            errorThrow("Creating temp render context fail.");

        // Define function pointer to create extended OpenGL context.
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 0;

        // Get the address of the wglCreateContextAttribsARB function.
        wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)(
            ::wglGetProcAddress("wglCreateContextAttribsARB"));

        // We don't need hRCTemp anymore.
        ::wglMakeCurrent(NULL, NULL);
        ::wglDeleteContext(hRCTemp);

        if (!wglCreateContextAttribsARB)
            errorThrow("wglCreateContextAttribsARB fail.");

        // Attributes for extended OpenGL context.
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };

        // Create extended context, supporting OpenGL 3.x.
        hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
        if (!hglrc || !::wglMakeCurrent(hdc, hglrc))
            errorThrow("Creating render context fail.");

        // Output information about current OpenGL version.
        int major, minor;
        ::glGetIntegerv(GL_MAJOR_VERSION, &major);
        ::glGetIntegerv(GL_MINOR_VERSION, &minor);
        log("OpenGL render context information:\n"
            "  Renderer       : %s\n"
            "  Vendor         : %s\n"
            "  Version        : %s\n"
            "  GLSL version   : %s\n"
            "  OpenGL version : %d.%d\n",
            (const char*)::glGetString(GL_RENDERER),
            (const char*)::glGetString(GL_VENDOR),
            (const char*)::glGetString(GL_VERSION),
            (const char*)::glGetString(GL_SHADING_LANGUAGE_VERSION),
            major, minor
        );

        if(!OpenGLInitExtensions())
            errorThrow("Init OpenGL extensions fail.");

        OPENGL_CHECK_FOR_ERRORS();
    }

    //-------------- Throw runtime_error --------------//
    void Graphics::errorThrow(std::string errorString)
    {
        std::stringstream errorMessage;
        errorMessage << errorString;
        errorMessage << " " << ::GetLastError() << '\n';

        throw std::runtime_error(errorMessage.str());
    }
}
