#include "GetImage.h"

XdoToolTask_GetImage::XdoToolTask_GetImage(XScreenshooter* screenshooter): XTask(screenshooter->GetDisplay()), screenshooter(screenshooter) {
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