#include "GetWindowPID.h"

using Nan::New;
using v8::Number;
using v8::Local;
using v8::Value;

XdoToolTask_GetWindowPID::XdoToolTask_GetWindowPID(
    xdo_t* xdo,
    Window window
): XdoToolTask_GetWindowProperty(xdo, window, "_NET_WM_PID") {

}

Local<Value> XdoToolTask_GetWindowPID::GetResult() {
    Local<Number> n = New<Number>(0);
    if(nitems > 0) {
        uint32_t pid = *((uint32_t*)value);
        n = New<Number>(pid);
    }
    return n;
}