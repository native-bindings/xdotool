#ifndef NODE_XDOTOOL_CLASS_CREATOR_H_
#define NODE_XDOTOOL_CLASS_CREATOR_H_

#include <map>
#include <nan.h>

using v8::Function;
using v8::Local;
using v8::Isolate;
using v8::FunctionTemplate;

using Nan::FunctionCallback;

class ClassCreator {
public:
    template<typename T>
    static Local<Function> NewClass(const char* className, std::map<std::string, FunctionCallback>& methods);
};

template<typename T>
Local<Function> ClassCreator::NewClass(const char* className, std::map<std::string, FunctionCallback>& methods) {
    Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();

    if(!T::constructor.IsEmpty()) {
        printf("Persistent not empty");
        return T::constructor.Get(isolate);
    }

    if(methods.find("constructor") == methods.end()) {
        throw std::runtime_error("'constructor' field on methods map is mandatory");
    }

    FunctionCallback Constructor = methods["constructor"];
    Local<FunctionTemplate> tpl = New<FunctionTemplate>(Constructor);
    tpl->SetClassName(New<String>(className).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(methods.size());

    for(auto & method : methods) {
        SetPrototypeMethod(tpl, method.first.c_str(), method.second);
    }

    T::constructor.Reset(tpl->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());

    return T::constructor.Get(isolate);
}

#endif // NODE_XDOTOOL_CLASS_CREATOR_H_
