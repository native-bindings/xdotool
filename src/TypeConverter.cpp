#include "TypeConverter.h"

#include <iostream>

using Nan::New;
using Nan::To;
using v8::String;
using v8::Local;
using v8::Value;
using v8::Object;
using v8::Int32;
using v8::Number;
using v8::Uint32;
using v8::Isolate;

bool TypeConverter::GetWindow(Local<Value> value,Window& out) {
    if(value->IsUndefined() || !value->IsString()) {
        return false;
    }

    auto size = Nan::DecodeBytes(value);
    char buffer[size];
    Nan::DecodeWrite(buffer, size, value);

    Window window = std::atol(buffer);
    if(!window) {
        return false;
    }

    out = window;
    return true;
}

bool TypeConverter::GetString(const Local<Value> value, std::string& out) {
    if(value->IsUndefined() || !value->IsString()) {
        return false;
    }
    const auto size = Nan::DecodeBytes(value);
    char buffer[size];
    Nan::DecodeWrite(buffer,size,value);
    out = std::string(buffer,size);
    return true;
}

/**
 * @deprecated
 */
bool TypeConverter::GetString(const Local<Value> obj, char** out) {
    if(obj->IsUndefined() || !obj->IsString()) {
        return false;
    }
    Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();
    const Local<String> title_string = Nan::To<String>(obj).ToLocalChecked();
    const int length = title_string->Utf8Length(isolate);
    const auto buffer = static_cast<char*>(malloc(sizeof(char) * (length + 1)));
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
bool TypeConverter::GetString(const Local<Object> obj, const char* prop, char** out) {
    const Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    return GetString(value, out);
}

bool TypeConverter::GetInt32(const Local<Object> obj, const char* prop, int32_t* out_n) {
    const Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsInt32()) {
        return false;
    }
    *out_n = Nan::To<Int32>(value).ToLocalChecked()->Value();
    return true;
}

bool TypeConverter::GetBool(const Local<Object> obj, const char* prop, bool* out_bool) {
    const Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsBoolean()) {
        return false;
    }
    *out_bool = value->BooleanValue(Nan::GetCurrentContext()->GetIsolate());
    return true;
}

bool TypeConverter::GetWindow(const Local<Object> obj, const char* prop, Window* window) {
    const Local<Value> window_str = Nan::Get(obj, New<String>(prop).ToLocalChecked()).ToLocalChecked();
    return GetWindow(window_str,*window);
}

bool TypeConverter::GetInt32(const Local<Value> val, int32_t& out) {
    if(!val->IsInt32()) return false;
    out = To<Int32>(val).ToLocalChecked()->Value();
    return true;
}

bool TypeConverter::GetUint32(const Local<Value> val, uint32_t& out) {
    if(!val->IsUint32()) return false;
    out = To<Uint32>(val).ToLocalChecked()->Value();
    return true;
}

bool TypeConverter::GetDouble(const Local<Value> val, double& out) {
    if(!val->IsNumber()) {
        return false;
    }
    out = Nan::To<Number>(val).ToLocalChecked()->Value();
    return true;
}
