#include "XTypeConverter.h"

#include <iostream>

using Nan::New;
using v8::String;
using v8::Isolate;

Window XTypeConverter::GetWindow(Local<Value> value) {
    if(value->IsUndefined() || !value->IsString()) {
        return 0;
    }

    Local<String> window_str = value->ToString(Nan::GetCurrentContext()).ToLocalChecked();
    char* window_str_value = new char[window_str->Utf8Length(Nan::GetCurrentContext()->GetIsolate())];
    window_str->WriteUtf8(Nan::GetCurrentContext()->GetIsolate(), window_str_value);
    Window window = (Window) atol(window_str_value);
    delete window_str_value;

    return window;  
}

bool XTypeConverter::GetString(Local<Value> value, char** out) {
    if(value->IsUndefined() || !value->IsString()) {
        return false;
    }
    Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();
    Local<String> title_string = Nan::To<String>(value).ToLocalChecked();
    int length = title_string->Utf8Length(isolate);
    auto buffer = (char*)malloc(sizeof(char)*(length + 1));
    if(buffer == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return false;
    }
    title_string->WriteUtf8(isolate, buffer);
    buffer[length] = 0;
    *out = buffer;
    return true;
}

/**
 * @returns true if string was found and allocated, false otherwise
 */
bool XTypeConverter::GetString(Local<Object> obj, const char* prop, char** out) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    return GetString(value, out);
}

bool XTypeConverter::GetInt32(Local<Object> obj, const char* prop, int32_t* out_n) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsInt32()) {
        return false;
    }
    if(!value->Int32Value(Nan::GetCurrentContext()).To(out_n)) {
        std::cerr << "Received empty value for property name: " << prop << std::endl;
    }
    return true;
}

bool XTypeConverter::GetBool(Local<Object> obj, const char* prop, bool* out_bool) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsBoolean()) {
        return false;
    }
    *out_bool = value->BooleanValue(Nan::GetCurrentContext()->GetIsolate());
    return true;
}

bool XTypeConverter::GetWindow(Local<Object> obj, const char* prop, Window* window) {
    Local<Value> window_str = Nan::Get(obj, New<String>(prop).ToLocalChecked()).ToLocalChecked();
    *window = GetWindow(window_str);
    if(*window == 0) {
        return false;
    }
    return true;
}
