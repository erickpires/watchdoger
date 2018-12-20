#include <node.h>
#include <v8.h>
#include <nan.h>

extern v8::Local<v8::Value> resetWatchdogerCore();
extern void initCore();

unsigned int watchdogerTimeout;
bool terminated;
v8::Isolate* isolate;
bool shouldThrowException = true;
bool useRealTime = false;

bool hasTimeoutCallback = false;
Nan::Callback timeoutCallback;

void interruptCallback(v8::Isolate* isolate, void* data) {
    Nan::RunScript(Nan::CompileScript(Nan::New<v8::String>(
        "process.nextTick(function () { throw new Error('Blocked event loop'); });"
        ).ToLocalChecked()
    ).ToLocalChecked());
}

NAN_METHOD(stopWatchdoger) {
	watchdogerTimeout = 0;
	terminated = 0;

    info.GetReturnValue().SetUndefined();
}

NAN_METHOD(resetWatchdoger) {
    Nan::EscapableHandleScope scope;
	if (info.Length() == 0 || Nan::To<v8::Uint32>(info[0]).IsEmpty()) {
        Nan::ThrowError("First agument must be an integer time threshold in milliseconds.");
        info.GetReturnValue().SetUndefined();
    } else  {
        unsigned int arg_value = Nan::To<v8::Uint32>(info[0]).ToLocalChecked()->Value();
        if (arg_value == 0) {
            Nan::ThrowError("The time threshold for blocking operations must be greater than 0.");
            info.GetReturnValue().SetUndefined();
        } else {
            stopWatchdoger(info);

            watchdogerTimeout = arg_value;
            isolate = info.GetIsolate();

            info.GetReturnValue().Set(resetWatchdogerCore());
        }
    }
}

NAN_METHOD(setShouldThrowException) {
    Nan::EscapableHandleScope scope;

    if(info.Length() == 0 || Nan::To<v8::Boolean>(info[0]).IsEmpty()) {
        Nan::ThrowError("First agument must be a boolean.");
        info.GetReturnValue().SetUndefined();
    } else {
        shouldThrowException = Nan::To<v8::Boolean>(info[0]).ToLocalChecked()->BooleanValue();
    }
}

NAN_METHOD(setUseRealTime)
{
    Nan::EscapableHandleScope scope;

    if(info.Length() == 0 || Nan::To<v8::Boolean>(info[0]).IsEmpty()) {
        Nan::ThrowError("First agument must be a boolean.");
        info.GetReturnValue().SetUndefined();
    } else {
        useRealTime = Nan::To<v8::Boolean>(info[0]).ToLocalChecked()->BooleanValue();
    }
}

NAN_METHOD(setTimeoutCallback) {
    Nan::EscapableHandleScope scope;

     if(info.Length() == 0 || Nan::To<v8::Function>(info[0]).IsEmpty()) {
        Nan::ThrowError("First agument must be a function.");
        info.GetReturnValue().SetUndefined();
    } else {
        hasTimeoutCallback = true;
        const v8::Local<v8::Function> func = Nan::To<v8::Function>(info[0]).ToLocalChecked();
        timeoutCallback.Reset(func);
    }
}

void cleanUpFunction(void*) {
    timeoutCallback.Reset();
}

NAN_MODULE_INIT(init) {
    initCore();
    isolate = v8::Isolate::GetCurrent();
    terminated = 0;
    Nan::Set(target,
        Nan::New<v8::String>("resetWatchdoger").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(resetWatchdoger)->GetFunction());
    Nan::Set(target,
        Nan::New<v8::String>("stopWatchdoger").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(stopWatchdoger)->GetFunction());
    Nan::Set(target,
        Nan::New<v8::String>("setShouldThrowException").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(setShouldThrowException)->GetFunction());
    Nan::Set(target,
        Nan::New<v8::String>("setUseRealTime").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(setUseRealTime)->GetFunction());
    Nan::Set(target,
        Nan::New<v8::String>("setTimeoutCallback").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(setTimeoutCallback)->GetFunction());

    node::AtExit(cleanUpFunction, NULL);
}

NODE_MODULE(watchdoger, init);
