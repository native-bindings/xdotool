#include "GetImage.h"

using v8::Local;
using v8::Value;
using v8::ArrayBuffer;

XdoToolTask_GetImage::XdoToolTask_GetImage(XScreenshooter* screenshooter): XTask(screenshooter->display), screenshooter(screenshooter) {
}

void XdoToolTask_GetImage::Execute() {
    try {
        screenshooter->GetImage();
    } catch(std::exception& e) {
        SetFailure(e.what());
    }
}

Local<Value> XdoToolTask_GetImage::GetResult() {
    Nan::EscapableHandleScope scope;
    Local<ArrayBuffer> buffer = ArrayBuffer::New(
        Nan::GetCurrentContext()->GetIsolate(),
        screenshooter->Data(),
        screenshooter->ByteLength(),
        v8::ArrayBufferCreationMode::kExternalized
    );
    return scope.Escape(buffer);
}