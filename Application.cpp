#include <iostream>
#include <stdexcept>
#include "Application.h"
#include "Window.h"
#include "math/Utility.h"

using namespace std;

namespace ST
{
    Application::Application()
        : lbutton_down(false)
    {
        graphics.SetCamera(&camera);
        timer.Reset();
        ShowWindow(MainWindow->GetHandle(), SW_NORMAL);
    }

    Application::~Application()
    {
    }

    int Application::Run()
    {
        MSG msg;
        while (true)
        {
            if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                    break;

                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                Sleep(1);
                logic();
            }
        }

        return msg.wParam;
    }

    //-------------- Draw a scene to the backbuffer --------------//
    void Application::logic()
    {
        timer.Reset();
        graphics.DrawScene();
    }

    void Application::KeyDown(int key)
    {
    }

    void Application::LButtonDown(size_t x, size_t y)
    {
    }

    void Application::LButtonUp(size_t x, size_t y)
    {
    }

    void Application::MouseMove(size_t x, size_t y)
    {
    }

    void Application::FileDropped(const string& filename, const string& ext)
    {
        if (ext == ".md5mesh")
        {
            model.LoadModel(filename + ext);
            graphics.LoadModel(model.GetPositions(),
                                model.GetNormals(), model.GetIndices());
        }
        else MessageBox(0, TEXT("File isn't a model or an animation."),
                        TEXT("ERROR"), MB_OK);
    }
}
