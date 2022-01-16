#include "GetWindowSize.h"

tasks::GetWindowSize::GetWindowSize(xdo_t* xdo, XID id): XdoToolTask(xdo),id(id) {

}

void tasks::GetWindowSize::Execute() {
    status = xdo_get_window_size(xdo,id,&width,&height);
    if(status != XDO_SUCCESS){
        SetFailure("Failed to get window size");
    }
}

v8::Local<v8::Value> tasks::GetWindowSize::GetResult() {
    auto obj = Nan::New<v8::Object>();
    Nan::Set(obj,Nan::New("width").ToLocalChecked(),Nan::New<v8::Uint32>(width));
    Nan::Set(obj,Nan::New("height").ToLocalChecked(),Nan::New<v8::Uint32>(height));
    return obj;
}
