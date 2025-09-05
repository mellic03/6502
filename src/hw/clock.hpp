#pragma once

#include <stdint.h>
#include <vector>


// class SignalEmitter;
// class SignalListener;



// class SignalEmitter
// {
// private:
//     friend class SignalListener;
//     std::vector<SignalListener*> listeners;
//     uint64_t mHertz;
//     uint64_t mNsecs;
//     uint64_t mPeriod;

// public:
//     SignalEmitter( uint64_t hz ): mHertz(hz), mNsecs(0), mPeriod(1000000000/hz) {  }
//     void update( uint64_t dt_msecs );

// };



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
//     virtual void Tick() = 0;
// };




// class SignalListenerExample
// {
// private:
//     const int mCycleRatio;
//     int mCycleCount;

// public:
//     SignalListenerExample()
//     :   mCycleRatio(4), mCycleCount(0) {  }

//     virtual void Tick() final
//     {
//         if (++mCycleCount >= mCycleRatio)
//         {
//             // this->UpdateDeviceOrSomething();
//             mCycleCount = 0;
//         }
//     }
// };

