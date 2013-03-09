// Include windows.h without WIN32_LEAN_AND_MEAN
// to support drag & drop.
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "Window.h"

using namespace std;

namespace ST
{
    Window* MainWindow;

    //-------------- Window constructor --------------//
    Window::Window(LPCTSTR title, DWORD style, DWORD exStyle,
                   int width, int height, int x, int y, HWND parent, HMENU menu)
    {
        keyEventProcessor = 0;

        WNDCLASSEX wc    = {0};
        HINSTANCE handle = ::GetModuleHandle(0);

        wc.cbSize        = sizeof(wc);
        wc.hCursor       = ::LoadCursor(0, IDC_ARROW);
        wc.hIcon         = ::LoadIcon(0, IDI_APPLICATION);
        wc.hInstance     = handle;
        wc.lpfnWndProc   = Window::windowProc;
        wc.lpszClassName = TEXT("AppWindowClass");
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        ::RegisterClassEx(&wc);

        RECT rect;
        rect.top = y;
        rect.left = x;
        rect.right = x + width;
        rect.bottom = y + height;

        ::AdjustWindowRectEx(&rect, style, (bool)menu, exStyle);

        hwnd = ::CreateWindowEx(exStyle, TEXT("AppWindowClass"),
                    title, style, x, y, rect.right - rect.left,
                    rect.bottom - rect.top, parent, menu, handle, this);
    }

    //-------------- Window Procedure --------------//
    LRESULT CALLBACK Window::windowProc(HWND hwnd, UINT msg,
                                        WPARAM wParam, LPARAM lParam)
    {
        static Window* windowInstance = 0; // What window do we process?

        // Before window is created, store the pointer in window's memory.
        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;
            windowInstance = (Window*)createStruct->lpCreateParams;

            // For now CreateWindowEx() hasn't returned HWND, so
            // windowInstance doesn't contain it's HWND. As we may need
            // this handle later, assign it's value beforehand.
            windowInstance->hwnd = hwnd;
        }

        if (windowInstance)
            return windowInstance->handleMessage(msg, wParam, lParam);
        else
            // This will happen exactly twice (before WM_NCCREATE message).
            return ::DefWindowProc(hwnd, msg, wParam, lParam);
    }

    //-------------- Message Handler --------------//
    LRESULT Window::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_KEYDOWN:
            {
                if (keyEventProcessor)
                    keyEventProcessor->KeyDown(wParam);
            } return 0;

            case WM_DROPFILES:
            {
                if (keyEventProcessor)
                {
                    HDROP hDrop = (HDROP)wParam;
                    const size_t size = 1000;
                    TCHAR file[size];
                    int fileNum = DragQueryFile(hDrop, -1, 0, 0);

                    if (fileNum > 2)
                    {
                        MessageBox(0, TEXT("To many files dropped."),
                                   TEXT("ERROR"), MB_OK);
                    }
                    else
                    {
                        // Go through all drag and dropped files.
                        for (int i = 0; i < fileNum; i++)
                        {
                            // Get the name of the file.
                            DragQueryFile(hDrop, i, file, size);

                            // That's why the whole
                            // project cannot work with UNICODE.
                            string fileName(file);

                            size_t dotPos = fileName.rfind(".");
                            if (dotPos == string::npos)
                            {
                                MessageBox(0, TEXT("File has bad extension."),
                                           TEXT("ERROR"), MB_OK);

                                break;
                            }

                            string name = fileName.substr(0, dotPos);
                            string ext = fileName.substr(dotPos);

                            keyEventProcessor->FileDropped(name, ext);
                        }
                    }

                    DragFinish(hDrop); // Release resources.
                }
            } return 0;

            case WM_LBUTTONDOWN:
            {
                size_t x = GET_X_LPARAM(lParam);
                size_t y = GET_Y_LPARAM(lParam);
                keyEventProcessor->LButtonDown(x, y);
            } return 0;

            case WM_LBUTTONUP:
            {
                size_t x = GET_X_LPARAM(lParam);
                size_t y = GET_Y_LPARAM(lParam);
                keyEventProcessor->LButtonUp(x, y);
            } return 0;

            case WM_MOUSEMOVE:
            {
                size_t x = GET_X_LPARAM(lParam);
                size_t y = GET_Y_LPARAM(lParam);
                keyEventProcessor->MouseMove(x, y);
            } return 0;

            case WM_DESTROY:
            {
                ::PostQuitMessage(0);
            } return 0;

            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        // Something is wrong.
        return true;
    }
}
