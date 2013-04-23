#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#include "Graphics.h"
#include "KeyEventProcessor.h"
#include "Timer.h"
#include "Model.h"

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
        virtual void FileDropped(const std::string& filename,
                                 const std::string& ext);
        int Run();

    private:
        void logic();

        bool lbutton_down;
        Timer timer;
        Model model;
        Graphics* graphics;
    };
}

#endif // APPLICATION_H_INCLUDED
