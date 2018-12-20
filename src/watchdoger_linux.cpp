#include <pthread.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/time.h>
#include <node.h>
#include <v8.h>
#include <nan.h>

#include "watchdoger.h"

class WatchdogerNativeLinux: public IWatchdogerNative {
	
    pthread_t watchdogerThread = 0;
    pthread_mutex_t watchdogerMutex;
    pthread_cond_t watchdogerCondition;

	struct timespec timeout;
    struct rusage start, end;

	public:

	WatchdogerNativeLinux(): 
        watchdogerMutex(PTHREAD_MUTEX_INITIALIZER), 
        watchdogerCondition(PTHREAD_COND_INITIALIZER) {}

	void startTimeCounter() {
		getrusage(RUSAGE_SELF, &start);
	}

    u32 elapsedExecutionTime() {
		getrusage(RUSAGE_SELF, &end);

        u32 elapsedMs = 
            ((end.ru_utime.tv_sec - start.ru_utime.tv_sec) + (end.ru_stime.tv_sec - start.ru_stime.tv_sec)) * 1000 +
            ((end.ru_utime.tv_usec - start.ru_utime.tv_usec) + (end.ru_stime.tv_usec - start.ru_stime.tv_usec)) / 1000;

        return elapsedMs;
	}

    // TODO(erick)!
	u32 elapsedRealTime() {
		return elapsedExecutionTime();
	}

    bool sleepFor(u32 remainingTime) {
        struct timespec timeout;
        int waitResult;

		pthread_mutex_lock(&watchdogerMutex);
        {
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += remainingTime / 1000;
            timeout.tv_nsec += (remainingTime % 1000) * 1000000;

            pthread_cond_timedwait(&watchdogerCondition, &watchdogerMutex, &timeout);
            waitResult = pthread_cond_timedwait(&watchdogerCondition, &watchdogerMutex, &timeout);
        }
        pthread_mutex_unlock(&watchdogerMutex);

        return waitResult == ETIMEDOUT;
	}

    bool sleepIndefinitely() {
		pthread_mutex_lock(&watchdogerMutex);
        auto waitResult = pthread_cond_wait(&watchdogerCondition, &watchdogerMutex);
        pthread_mutex_unlock(&watchdogerMutex);

        return waitResult == ETIMEDOUT;
	}

	bool startWorkerThread(void* (*watchdogerWorker) (void*)) {
    	auto result = pthread_create(&watchdogerThread, NULL, watchdogerWorker, NULL);
        return result != 0;
	}

    void signalWorkerThread() {
    	pthread_mutex_lock(&watchdogerMutex);
    	pthread_cond_signal(&watchdogerCondition);
        pthread_mutex_unlock(&watchdogerMutex);
	}
};

IWatchdogerNative* WatchdogerNativeFactory() {
	return new WatchdogerNativeLinux();
}