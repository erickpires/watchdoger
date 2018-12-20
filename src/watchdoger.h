#ifndef WATCHDOGER_H
#define WATCHDOGER_H 1
#include <stdint.h>

typedef uint32_t u32;

class IWatchdogerNative {
    public:
    virtual void startWorkerThread(void* (*watchdogerWorker) (void*)) = 0;
    virtual void signalWorkerThread() = 0;

    virtual bool startTimeCounter() = 0;
    virtual u32 elapsedExecutionTime() = 0;
    virtual u32 elapsedRealTime() = 0;
    virtual bool sleepFor(u32 time) = 0;
    virtual bool sleepIndefinitely() = 0;
};

#endif