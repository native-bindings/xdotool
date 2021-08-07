#ifndef NODE_XDOTOOL_CLASS_CREATOR_H_
#define NODE_XDOTOOL_CLASS_CREATOR_H_

#include <map>
#include <nan.h>

class ClassCreator {
public:
    template<typename T>
    static v8::Local<v8::Function> NewClass(std::string className, std::map<std::string, Nan::FunctionCallback>& methods);
};

template<typename T>
v8::Local<v8::Function> ClassCreator::NewClass(std::string className, std::map<std::string, Nan::FunctionCallback>& methods) {
//    v8::Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();
//
//    if(!T::constructor.IsEmpty()) {
//        printf("Persistent not empty");
//        return T::constructor.Get(isolate);
//    }

    if(methods.find("constructor") == methods.end()) {
        throw std::runtime_error("'constructor' field on methods map is mandatory");
    }

    Nan::FunctionCallback Constructor = methods["constructor"];
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(Constructor);
    tpl->SetClassName(Nan::New<v8::String>(className).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(methods.size());

    for(auto & method : methods) {
        SetPrototypeMethod(tpl, method.first.c_str(), method.second);
    }

    T::constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    return Nan::GetFunction(tpl).ToLocalChecked();
}

#endif // NODE_XDOTOOL_CLASS_CREATOR_H_
