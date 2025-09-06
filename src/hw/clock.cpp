#include "clock.hpp"


bool HwClock::tick( uint64_t dt_msecs )
{
    // 1 Hz = 1000 ms
    // 2 Hz =  500 ms
    // 4 Hz =  250 ms
    // static constexpr size_t HZ = 1'790'000;
    // static constexpr size_t NS = 1'000'000'000 / HZ;

    m_accum += 1'000'000'000 * dt_msecs;

    if (m_accum >= m_period)
    {
        m_accum = 0;
        return true;
    }

    return false;
}



// void SignalListener::Listen( SignalEmitter &emitter )
// {
//     emitter.listeners.push_back(this);
// }


// void SignalListener::_tick()
// {
//     if ((++mCycleCount) >= mDivisor)
//     {
//         this->Tick();
//         mCycleCount = 0;
//     }
// }
