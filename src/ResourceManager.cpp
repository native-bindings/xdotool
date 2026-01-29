#include <nan.h>

#include "ResourceManager.h"

void ResourceManager::AddResource(void* ptr) {
    pointers.emplace_back(ptr);
}

ResourceManager::~ResourceManager() {
    for(auto & p : pointers) {
        free(p);
    }
}


static void FreeCallback(char*, void* hint) {
}

v8::Local<v8::Object> CreateExternalArrayBuffer(char* nativeData, std::uint32_t len) {
    Nan::HandleScope scope;

    // Create externalized ArrayBuffer
    v8::Local<v8::Object> buffer = Nan::NewBuffer(
        nativeData,
        len,
        FreeCallback,
        nullptr
    ).ToLocalChecked();

    return buffer;
}