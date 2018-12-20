#include <node.h>
#include <v8.h>
#include <nan.h>

#include "watchdoger.h"

extern unsigned int watchdogerTimeout;
extern bool useRealTime;
extern bool shouldThrowException;

extern bool hasTimeoutCallback;
extern bool terminated;
extern v8::Isolate* isolate;

extern bool hasTimeoutCallback;
extern Nan::Callback timeoutCallback;


extern IWatchdogerNative* WatchdogerNativeFactory();

extern void interruptCallback(v8::Isolate *isolate, void *data);


IWatchdogerNative* watchdogerNative;

v8::Local<v8::Value> argv[] {Nan::Null()};
void timeoutCallbackCaller(v8::Isolate* isolate, void* data) {
   Nan::Call(timeoutCallback, 1, argv);
}

void* watchdogerWorker(void* data) {
    bool skipTimeCapture = false;

    u32 elapsedMs = 0;

	while (true) {
		if (skipTimeCapture) {
			skipTimeCapture = false;
        } else {
            watchdogerNative->startTimeCounter();
        }

        bool sleepResult;
		if(watchdogerTimeout == 0) {
            sleepResult = watchdogerNative->sleepIndefinitely();
        } else {
            u32 remainingTime = watchdogerTimeout < elapsedMs ? 0 : watchdogerTimeout - elapsedMs;
            sleepResult = watchdogerNative->sleepFor(remainingTime);
        }

		if (sleepResult) {
			if (watchdogerTimeout > 0) {
                if(!useRealTime) {
				    elapsedMs = watchdogerNative->elapsedExecutionTime();
                } else {
                    elapsedMs = watchdogerNative->elapsedRealTime();
                }
				

				if (elapsedMs >= watchdogerTimeout) {
					terminated = 1;
                    isolate->TerminateExecution();

					if(hasTimeoutCallback) {
						isolate->RequestInterrupt(timeoutCallbackCaller, NULL);
					}

					if(shouldThrowException) {
                    	isolate->RequestInterrupt(interruptCallback, NULL);
					}
				} else {
					skipTimeCapture = true;
				}
			}
		}
	}

    return NULL;
}

v8::Local<v8::Value> resetWatchdogerCore() {
    Nan::EscapableHandleScope scope;
    
    if (watchdogerNative == NULL) {
        watchdogerNative = WatchdogerNativeFactory();
        watchdogerNative->startWorkerThread(watchdogerWorker);
    } else {
    	watchdogerNative->signalWorkerThread();
    }

    return scope.Escape(Nan::Undefined());
}

void initCore() {
	watchdogerNative = NULL;
}
