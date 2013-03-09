#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

namespace ST
{
    /* Timer class is destined to measure
     * elapsed time.
     * Cannot be used before calling Reset().
     */
    class Timer
    {
    public:
        Timer();

        void Reset();
        double ElapsedTime() const;
    private:
        __int64 start;
        __int64 frequency;
    };
}


#endif // TIMER_H_INCLUDED
