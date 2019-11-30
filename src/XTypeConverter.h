#ifndef NODE_XDOTOOL_XTYPECONVERTER_H_
#define NODE_XDOTOOL_XTYPECONVERTER_H_

#include "xdo_c.h"

using v8::Local;
using v8::Object;
using v8::Value;

class XTypeConverter {
public:
    /**
     * @returns true if string was found and allocated, false otherwise
     */
    static bool GetString(Local<Object>, const char*, char**);
    static bool GetInt32(Local<Object>, const char*, int32_t*);
    static bool GetBool(Local<Object>, const char*, bool*);
    static Window GetWindow(Local<Value> value);
    static bool GetWindow(Local<Object>, const char*, Window*);
    static bool GetString(Local<Value> obj, char** out);
};

#endif // NODE_XDOTOOL_XTYPECONVERTER_H_
