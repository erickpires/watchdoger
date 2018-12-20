#include <node.h>
#include <v8.h>
#include <nan.h>

IWatchdogerNative* WatchdogerNativeFactory() {
    Nan::ThrowError("Unimplemented MAC native watchdoger.");
	return new NULL;
}