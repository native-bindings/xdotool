#include "GetViewportDimensions.h"

using Nan::New;
using Nan::Set;
using v8::Number;
using v8::String;
using v8::Local;
using v8::Value;
using v8::Object;

tasks::GetViewportDimensions::GetViewportDimensions(xdo_t* xdo, int screen): XdoToolTask(xdo), screen(screen) {

}

void tasks::GetViewportDimensions::Execute() {
    if(xdo_get_viewport_dimensions(xdo, &width, &height, screen) != XDO_SUCCESS) {
        SetFailure("Failed to get viewport dimensions");
    }
}

Local<Value> tasks::GetViewportDimensions::GetResult() {
    Local<Object> obj = New<Object>();
    Set(obj, New<String>("width").ToLocalChecked(), New<Number>(width));
    Set(obj, New<String>("height").ToLocalChecked(), New<Number>(height));
    return obj;
}