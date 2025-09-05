// #include "clock.hpp"


// void SignalEmitter::update( uint64_t dt_msecs )
// {
//     // 1 Hz = 1000 ms
//     // 2 Hz =  500 ms
//     // 4 Hz =  250 ms
//     // static constexpr size_t HZ = 1'790'000;
//     // static constexpr size_t NS = 1'000'000'000 / HZ;
//     mNsecs += 1000000*dt_msecs;

//     if (mNsecs < mPeriod)
//     {
//         return; 
//     }

//     for (auto *L: listeners)
//     {
//         L->_tick();
//     }

//     mNsecs = 0;
// }



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
