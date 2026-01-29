#include "Keyboard.h"
#include "XdoTool.h"
#include "tasks/QueryKeymap.h"
#include "XdoToolTaskWorker.h"
#include "ClassCreator.h"
#include "TypeConverter.h"

#include <X11/XKBlib.h>

Nan::Persistent<v8::Function> XKeyboard::constructor;

XKeyboard::XKeyboard(Display* display): XService(display), keys{} {
}

XKeyboard::~XKeyboard() = default;

void XKeyboard::Init(v8::Local<v8::Object> exports) {
    std::map<std::string, Nan::FunctionCallback> methods {
        { "constructor", Constructor },
        { "queryKeymap", QueryKeymap },
        { "keycodeToKeysym", KeycodeToKeysym },
        { "keysymToString", KeysymToString },
        { "getActiveKeysToKeycodeList", GetActiveKeysToKeycodeList }
    };
    Nan::Set(
        exports,
        Nan::New<v8::String>("Keyboard").ToLocalChecked(),
        ClassCreator::NewClass<XKeyboard>("Keyboard", methods)
    );
}

NAN_METHOD(XKeyboard::KeycodeToKeysym) {
    uint32_t keycode;
    XKeyboard* kbd = nullptr;
    if(!TypeConverter::GetUint32(info[0],keycode)) {
        Nan::ThrowError("First argument must be an integer");
        return;
    }
    if(!TypeConverter::Unwrap<XKeyboard>(info.This(),&kbd)) {
        Nan::ThrowError("Method called under invalid context.");
        return;
    }
    const KeySym keysym = XkbKeycodeToKeysym(kbd->display, keycode, 0, 0);
    info.GetReturnValue().Set(Nan::New<v8::Number>(keysym));
}

NAN_METHOD(XKeyboard::KeysymToString) {
    uint32_t keysym;
    if(!TypeConverter::GetUint32(info[0],keysym)){
        Nan::ThrowError("First argument must be a valid keysym");
        return;
    }
    char* result = XKeysymToString(keysym);
    if(!result) {
        Nan::ThrowError(std::string("Keysym " + std::to_string(keysym) + " does not exist").c_str());
        return;
    }
    info.GetReturnValue().Set(Nan::New<v8::String>(result).ToLocalChecked());
}

NAN_METHOD(XKeyboard::Constructor) {
    XdoTool* tool;
    if(!TypeConverter::Unwrap<XdoTool>(info[0],&tool)){
        Nan::ThrowError("First argument must be a valid instance of XdoTool");
        return;
    }
    auto keyboard = new XKeyboard(tool->GetXdo()->xdpy);
    keyboard->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(XKeyboard::QueryKeymap) {
    XKeyboard* kbd = nullptr;
    if(!TypeConverter::Unwrap(info.This(),&kbd)) {
        Nan::ThrowError("Method called under invalid context.");
        return;
    }
    const auto task = new XTask_QueryKeymap(kbd);
    const auto callback = new Nan::Callback(Nan::To<v8::Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

v8::Local<v8::Value> XKeyboard::GetBuffer() {
    if(arrayBuffer.IsEmpty()) {
        auto buffer = CreateExternalArrayBuffer(keys, 32);
        arrayBuffer.Reset(buffer);
    }
    return Nan::New(arrayBuffer);
}

NAN_METHOD(XKeyboard::GetActiveKeysToKeycodeList) {

}
