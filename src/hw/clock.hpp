#pragma once

#include <stdint.h>
#include <vector>


// class SignalEmitter;
// class SignalListener;



class HwClock
{
private:
    uint64_t m_hz;
    uint64_t m_accum;
    uint64_t m_period;

public:
    HwClock( uint64_t hz ): m_hz(hz), m_accum(0), m_period(1000000000/hz) {  }
    bool tick( uint64_t dt_msecs );

};



// class SignalListener
// {
// private:
//     friend class SignalEmitter;
//     const size_t mDivisor;
//     size_t mCycleCount;
//     void _tick();

// public:
//     SignalListener( uint16_t divisor )
//     :   mDivisor(divisor), mCycleCount(0) {  };
//     void Listen( SignalEmitter& );
//     virtual void tick() = 0;
// };




// class SignalListenerExample
// {
// private:
//     const int mCycleRatio;
//     int mCycleCount;

// public:
//     SignalListenerExample()
//     :   mCycleRatio(4), mCycleCount(0) {  }

//     virtual void tick() final
//     {
//         if (++mCycleCount >= mCycleRatio)
//         {
//             // this->UpdateDeviceOrSomething();
//             mCycleCount = 0;
//         }
//     }
// };

