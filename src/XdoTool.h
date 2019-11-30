#ifndef NODE_XDOTOOL_XDOTOOL_H_
#define NODE_XDOTOOL_XDOTOOL_H_

#include "XService.h"

using v8::Object;
using v8::Local;
using Nan::Persistent;
using v8::Function;

class XdoTool : public XService {
public:
    XdoTool(xdo_t* xdo);
    ~XdoTool();
    static void Init(Local<Object> exports);
    xdo_t* GetXdo() {return xdo;}
    static Persistent<Function> constructor;
private:
    xdo_t* xdo;
    static NAN_METHOD(Constructor);
    static NAN_METHOD(SearchWindows);
    static NAN_METHOD(GetWindowPID);
    static NAN_METHOD(GetMouseLocation);
    static NAN_METHOD(GetViewportDimensions);
    static NAN_METHOD(MoveMouse);
    static NAN_METHOD(ActivateWindow);
    static NAN_METHOD(QueryKeymap);
    static NAN_METHOD(GetImage);
    static NAN_METHOD(WindowHasProperty);
};


#endif // NODE_XDOTOOL_XDOTOOL_H_