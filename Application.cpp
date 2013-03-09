#include <stdexcept>
#include "Application.h"
#include "Window.h"
#include "math/Utility.h"
#include <iostream>

using namespace std;

namespace ST
{
    Application::Application()
        : lbutton_down(false)
    {
        graphics = new Graphics();
        timer.Reset();
        ShowWindow(MainWindow->GetHandle(), SW_NORMAL);
    }

    Application::~Application()
    {
        if (graphics)
        {
            delete graphics;
            graphics = 0;
        }
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
                gameLogic();
            }
        }

        return msg.wParam;
    }

    //-------------- Draw a scene to the backbuffer --------------//
    void Application::gameLogic()
    {
        graphics->Update(timer.ElapsedTime());
        timer.Reset();

        graphics->DrawScene();
    }

    void Application::KeyDown(int key)
    {
        // VK_LEFT
        if (key == VK_RIGHT)
            graphics->Rotate(Math::DEG_2_RAD);
        else if (key == VK_LEFT)
            graphics->Rotate(-Math::DEG_2_RAD);
        else if (key == 74) // 'J'
            graphics->AffectJoint();
    }

    void Application::LButtonDown(size_t x, size_t y)
    {
        lbutton_down = true;
        graphics->GetJoint(x, y);
    }

    void Application::LButtonUp(size_t x, size_t y)
    {
        lbutton_down = false;
    }

    void Application::MouseMove(size_t x, size_t y)
    {
        if (lbutton_down)
        {
        }
    }

    void Application::FileDropped(const string& fileName, const string& ext)
    {
        if (ext == ".md5mesh")
        {
            graphics->AddModel(fileName + ext);
        }
        else if (ext == ".md5anim")
        {
            try
            {
                graphics->AddAnimation(fileName + ext);
            }
            catch (const runtime_error& msg)
            {
                MessageBox(0, msg.what(), TEXT("ERROR"), MB_OK);
            }
        }
        else
        {
            MessageBox(0, TEXT("File isn't a model or an animation."),
                TEXT("ERROR"), MB_OK);
        }
    }
}
