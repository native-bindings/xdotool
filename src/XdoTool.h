#ifndef NODE_XDOTOOL_XDOTOOL_H_
#define NODE_XDOTOOL_XDOTOOL_H_

#include "XService.h"

class XdoTool : public XService {
public:
    explicit XdoTool(xdo_t* xdo);
    ~XdoTool() override;
    static void Init(v8::Local<v8::Object> exports);
    [[nodiscard]] xdo_t* GetXdo() const {return xdo;}
    static Nan::Persistent<v8::Function> constructor;
private:
    xdo_t* xdo;
    static NAN_METHOD(Constructor);
    static NAN_METHOD(SearchWindows);
    static NAN_METHOD(GetWindowPID);
    static NAN_METHOD(GetWindowSize);
    static NAN_METHOD(GetFocusedWindow);
    static NAN_METHOD(GetMouseLocation);
    static NAN_METHOD(SendKeysequence);
    static NAN_METHOD(GetViewportDimensions);
    static NAN_METHOD(MoveMouse);
    static NAN_METHOD(ActivateWindow);
    static NAN_METHOD(EnterText);
    static NAN_METHOD(Sync);
    static NAN_METHOD(WindowHasProperty);
};


#endif // NODE_XDOTOOL_XDOTOOL_H_