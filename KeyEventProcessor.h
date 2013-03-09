#ifndef KEYEVENTPROCESSOR_H_INCLUDED
#define KEYEVENTPROCESSOR_H_INCLUDED

#include <string>

namespace ST
{
    struct KeyEventProcessor
    {
        virtual void LButtonDown(size_t x, size_t y) = 0;
        virtual void LButtonUp(size_t x, size_t y) = 0;
        virtual void MouseMove(size_t x, size_t y) = 0;
        virtual void KeyDown(int key) = 0;
        virtual void FileDropped(const std::string& fileName,
                                 const std::string& ext) = 0;
    };
}

#endif // KEYEVENTPROCESSOR_H_INCLUDED
