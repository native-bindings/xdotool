#include "WindowHasProperty.h"

using Nan::New;
using v8::Boolean;

XdoToolTask_WindowHasProperty::XdoToolTask_WindowHasProperty(
    xdo_t* xdo,
    Window window,
    const char* property
): XdoToolTask_GetWindowProperty(xdo, window, property), window(window) {

}

Local<Value> XdoToolTask_WindowHasProperty::GetResult() {
    if(nitems > 0) {
        return New<Boolean>(true);
    }
    return New<Boolean>(false);
}