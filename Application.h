#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "Graphics.h"
#include "KeyEventProcessor.h"
#include "Timer.h"

namespace ST
{
    class Application : public KeyEventProcessor
    {
    public:
        Application();
        virtual ~Application();

        virtual void LButtonDown(size_t x, size_t y);
        virtual void LButtonUp(size_t x, size_t y);
        virtual void MouseMove(size_t x, size_t y);
        virtual void KeyDown(int key);
        virtual void FileDropped(const std::string& fileName,
                                 const std::string& ext);
        int Run();

    private:
        void gameLogic();

        bool lbutton_down;
        Timer timer;
        Graphics* graphics;
    };
}

#endif // APPLICATION_H_INCLUDED
