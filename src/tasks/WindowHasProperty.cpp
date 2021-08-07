#include "WindowHasProperty.h"

using Nan::New;
using v8::Boolean;
using v8::Local;
using v8::Value;

XdoToolTask_WindowHasProperty::XdoToolTask_WindowHasProperty(
    xdo_t* xdo,
    Window window,
    std::string property
): XdoToolTask_GetWindowProperty(xdo, window, property), window(window) {

}

Local<Value> XdoToolTask_WindowHasProperty::GetResult() {
    if(nitems > 0) {
        return New<Boolean>(true);
    }
    return New<Boolean>(false);
}