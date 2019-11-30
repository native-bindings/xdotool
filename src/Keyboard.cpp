#include "Keyboard.h"
#include "XdoTool.h"
#include "tasks/QueryKeymap.h"
#include "XdoToolTaskWorker.h"
#include "ClassCreator.h"

#include <X11/XKBlib.h>

using v8::Number;
using v8::String;
using v8::ArrayBufferCreationMode;

using Nan::To;
using Nan::Set;
using Nan::Callback;
using Nan::New;

Persistent<Function> XKeyboard::constructor;

XKeyboard::XKeyboard(Display* display): XService(display) {

}

XKeyboard::~XKeyboard() {
    arrayBuffer.Reset();
}

char* XKeyboard::Keys() {
    return keys;
}

void XKeyboard::Init(Local<Object>exports) {
    std::map<std::string, FunctionCallback> methods {
        { "constructor", Constructor },
        { "queryKeymap", QueryKeymap },
        { "keycodeToKeysym", KeycodeToKeysym },
        { "keysymToString", KeysymToString }
    };
    Set(
        exports,
        New<String>("Keyboard").ToLocalChecked(),
        ClassCreator::NewClass<XKeyboard>("Keyboard", methods)
    );
}

NAN_METHOD(XKeyboard::KeycodeToKeysym) {
    uint32_t keycode;
    if(!info[0]->Uint32Value(Nan::GetCurrentContext()).To(&keycode)) {
        Nan::ThrowError("First argument must be a valid keycode");
        return;
    }
    XKeyboard* kbd = Unwrap<XKeyboard>(info.This());
    KeySym keysym = XkbKeycodeToKeysym(kbd->display, keycode, 0, 0);
    info.GetReturnValue().Set(New<Number>(keysym));
}

NAN_METHOD(XKeyboard::KeysymToString) {
    uint32_t keysym;
    if(!info[0]->Uint32Value(Nan::GetCurrentContext()).To(&keysym)) {
        Nan::ThrowError("First argument must be a valid keysym");
        return;
    }
    info.GetReturnValue().Set(New<String>(XKeysymToString(keysym)).ToLocalChecked());
}

NAN_METHOD(XKeyboard::Constructor) {
    if(info[0]->IsUndefined() || !info[0]->IsObject()) {
        Nan::ThrowError("First argument must be a valid instance of XdoTool");
        return;
    }

    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info[0]->ToObject(Nan::GetCurrentContext()->GetIsolate()));
    auto keyboard = new XKeyboard(tool->GetXdo()->xdpy);
    keyboard->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(XKeyboard::QueryKeymap) {
    auto kbd = Nan::ObjectWrap::Unwrap<XKeyboard>(info.This());
    auto task = new XTask_QueryKeymap(kbd);
    auto callback = new Callback(To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

Local<ArrayBuffer> XKeyboard::GetArrayBuffer() {
    Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();
    if(!arrayBuffer.IsEmpty()) {
        return New(arrayBuffer);
    }
    Local<ArrayBuffer> buffer = ArrayBuffer::New(
        Nan::GetCurrentContext()->GetIsolate(),
        keys,
        32,
        ArrayBufferCreationMode::kExternalized
    );
    arrayBuffer.Reset(buffer);
    return New(arrayBuffer);
}
