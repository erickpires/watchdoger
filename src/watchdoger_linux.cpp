#include <node.h>
#include <v8.h>
#include <nan.h>

IWatchdogerNative* WatchdogerNativeFactory() {
    Nan::ThrowError("Unimplemented Linux native watchdoger.");
	return new NULL;
}