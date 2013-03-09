#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include "KeyEventProcessor.h"

namespace ST
{
    class Window;

    extern Window* MainWindow;

    /** A singleton system window.
    */
    class Window
    {
    public:

        /** Initialization of the window instance.
            @param title   The name of the window.
            @param style   The style of the window.
            @param width   The width, in device units, of the window.
            @param height  The height, in device units, of the window.
            @param x       The initial horizontal position of the window.
            @param y       The initial vertical position of the window.
            @param exStyle The extended window style of the window.
            @param parent  A handle to the parent or owner of this window.
            @param menu    A handle to a menu.
        */
        Window (LPCTSTR title,
                DWORD  style,
                DWORD  exStyle = 0,
                int    width   = 800,
                int    height  = 600,
                int    x       = 50,
                int    y       = 50,
                HWND   parent  = 0,
                HMENU  menu    = 0
               );

        /** Returns a handle to a created window.
        */
        HWND GetHandle() const
        {
            return hwnd;
        }

        /** Key events will be sent to processor.
        */
        void SetKeyEventProcessor(KeyEventProcessor* processor)
        {
            keyEventProcessor = processor;
        }

    private:
        static LRESULT CALLBACK windowProc(HWND hwnd,
                                           UINT msg,
                                           WPARAM wParam,
                                           LPARAM lParam);

        LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        static const LPCWSTR className;
        static size_t countOfWindows;

        HWND hwnd;
        KeyEventProcessor* keyEventProcessor;
    };
}

#endif // WINDOW_H_INCLUDED
