#include <process.h>
#include <sys/timeb.h>
#include <node.h>
#include <v8.h>
#include <nan.h>

#include "watchdoger.h"

typedef void (*windows_thread)(void *);

class WatchdogerNativeWin: public IWatchdogerNative {
	HANDLE scriptThread = NULL;
	HANDLE watchdogerThread = NULL;
	HANDLE event = NULL;

	ULARGE_INTEGER su, sk, eu, ek;
    struct timeb startRealTime, _elapsedRealTime;
	FILETIME tmp;

	public:

	WatchdogerNativeWin() {
		if (NULL == (event = CreateEvent(NULL, FALSE, FALSE, NULL))) {
    		Nan::ThrowError("Unable to initialize a watchdoger thread.");
    	}

    	if (!DuplicateHandle(
	    		GetCurrentProcess(), 
	    		GetCurrentThread(), 
	    		GetCurrentProcess(),
	    		&scriptThread,
	    		0,
	    		FALSE,
	    		DUPLICATE_SAME_ACCESS)) 
    	{
    		CloseHandle(event);
    		event = NULL;
    		Nan::ThrowError("Unable to duplicate handle of the script thread.");
    	}
	}

	void startTimeCounter() {
			GetThreadTimes(scriptThread, &tmp, &tmp, (LPFILETIME)&sk.u, (LPFILETIME)&su.u);
            ftime(&startRealTime);
	}

    u32 elapsedExecutionTime() {
		GetThreadTimes(scriptThread, &tmp, &tmp, (LPFILETIME)&ek.u, (LPFILETIME)&eu.u);
		ULONGLONG elapsed100Ns = ek.QuadPart - sk.QuadPart + eu.QuadPart - su.QuadPart;

		// Thread execution times are reported in 100ns units. Convert to milliseconds.
		return u32 (elapsed100Ns / 10000);
	}

	u32 elapsedRealTime() {
		ftime(&_elapsedRealTime);
        return (unsigned int)(1000.0 * (_elapsedRealTime.time - startRealTime.time) + (_elapsedRealTime.millitm - startRealTime.millitm));
	}

    bool sleepFor(u32 time) {
		return WaitForSingleObject(event, time) == WAIT_TIMEOUT;
	}

    bool sleepIndefinitely() {
		return WaitForSingleObject(event, INFINITE) == WAIT_TIMEOUT;
	}

	void startWorkerThread(void* (*watchdogerWorker) (void*)) {
    	if ((watchdogerThread = (HANDLE)_beginthread((windows_thread)watchdogerWorker, 4096, NULL)) == NULL) {
    		CloseHandle(event);
    		event = NULL;
      		CloseHandle(scriptThread);
      		scriptThread = 0;
      		Nan::ThrowError("Unable to initialize a watchdoger thread.");
    	}
	}

    void signalWorkerThread() {
    	SetEvent(event);
	}
};

IWatchdogerNative* WatchdogerNativeFactory() {
	return new WatchdogerNativeWin();
}