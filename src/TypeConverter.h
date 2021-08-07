#ifndef NODE_XDOTOOL_XTYPECONVERTER_H_
#define NODE_XDOTOOL_XTYPECONVERTER_H_

#include "xdo_c.h"

class TypeConverter {
public:
    /**
     * @returns true if string was found and allocated, false otherwise
     */
    static bool GetString(v8::Local<v8::Object>, const char*, char**);
    static bool GetInt32(v8::Local<v8::Object>, const char*, int32_t*);
    static bool GetInt32(v8::Local<v8::Value>, int32_t&);
    static bool GetDouble(v8::Local<v8::Value>, double&);
    static bool GetUint32(v8::Local<v8::Value> val, uint32_t&);
    static bool GetBool(v8::Local<v8::Object>, const char*, bool*);
    static bool GetWindow(v8::Local<v8::Value> value, Window&);
    static bool GetString(v8::Local<v8::Value> value, std::string& out);
    static bool GetWindow(v8::Local<v8::Object>, const char*, Window*);
    static bool GetString(v8::Local<v8::Value> obj, char** out);
    template<typename T>
    static bool Unwrap(v8::Local<v8::Value> obj, T** out) {
        Nan::Persistent<v8::Function>& constructor = T::constructor;
        if(!obj->InstanceOf(Nan::GetCurrentContext(),Nan::To<v8::Object>(Nan::New(constructor)).ToLocalChecked()).ToChecked()){
            return false;
        }
        *out = Nan::ObjectWrap::Unwrap<T>(Nan::To<v8::Object>(obj).ToLocalChecked());
        return true;
    }

};

#endif // NODE_XDOTOOL_XTYPECONVERTER_H_
