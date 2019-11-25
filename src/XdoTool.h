#ifndef NODE_XDOTOOL_XDOTOOL_H_
#define NODE_XDOTOOL_XDOTOOL_H_

#include "xdo_c.h"

using v8::Object;
using v8::Value;
using v8::Local;

class XdoTool : public Nan::ObjectWrap {
public:
    XdoTool(xdo_t* xdo);
    ~XdoTool();
    static void Init(Local<Object> exports);
private:
    xdo_t* xdo;
    static NAN_METHOD(Constructor);
    /**
     * @returns true if string was found and allocated, false otherwise
     */
    static bool GetString(Local<Object>, const char*, char**);
    static bool GetInt32(Local<Object>, const char*, int32_t*);
    static bool GetBool(Local<Object>, const char*, bool*);
    static bool GetString(Local<Value> obj, char** out);
    static NAN_METHOD(SearchWindows);
    static NAN_METHOD(GetWindowPID);
    static NAN_METHOD(GetMouseLocation);
    static NAN_METHOD(GetViewportDimensions);
    static NAN_METHOD(MoveMouse);
    static NAN_METHOD(ActivateWindow);
    static NAN_METHOD(WindowHasProperty);
};


#endif // NODE_XDOTOOL_XDOTOOL_H_