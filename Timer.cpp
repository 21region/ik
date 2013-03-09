#include "Timer.h"

namespace ST
{
    //--------- Init of internal variables ---------//
    Timer::Timer() : start(0)
    {
        LARGE_INTEGER freq;
        ::QueryPerformanceFrequency(&freq);

        frequency = freq.QuadPart;
    }

    //--------- Init timer ---------//
    void Timer::Reset()
    {
        LARGE_INTEGER tempStart;
        ::QueryPerformanceCounter(&tempStart);

        start = tempStart.QuadPart;
    }
    //--------- Returns elapsed time in seconds ---------//
    double Timer::ElapsedTime() const
    {
        LARGE_INTEGER currentTime;
        ::QueryPerformanceCounter(&currentTime);

        return (double)(currentTime.QuadPart - start) / frequency;
    }
}
