#include "GetMouseLocation.h"

using v8::Local;
using v8::Value;
using v8::Number;
using v8::Object;
using v8::String;
using Nan::New;
using Nan::Set;

XdoToolTask_GetMouseLocation::XdoToolTask_GetMouseLocation(xdo_t* xdo): XdoToolTask(xdo) {}

void XdoToolTask_GetMouseLocation::Execute() {
    if(xdo_get_mouse_location(xdo, &x, &y, &screen_num) != XDO_SUCCESS) {
        SetFailure("Failed to get mouse location");
    }
}
Local<Value> XdoToolTask_GetMouseLocation::GetResult() {
    Local<Object> result = New<Object>();
    std::vector<std::pair<const char*, int>> values {
        { "x", x },
        { "y", y },
        { "screen_num", screen_num }
    };
    for(auto & v : values) {
        Set(result, Nan::New<String>(v.first).ToLocalChecked(), Nan::New<Number>(v.second));
    }
    return result;
}