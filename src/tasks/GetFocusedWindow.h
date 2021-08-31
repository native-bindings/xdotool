#ifndef NODE_XDOTOOL_GETFOCUSEDWINDOW_H
#define NODE_XDOTOOL_GETFOCUSEDWINDOW_H


#include "../XdoToolTask.h"

class XdoToolTask_GetFocusedWindow : public XdoToolTask {
public:
    explicit XdoToolTask_GetFocusedWindow(xdo_t*);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    Window result;
};


#endif //NODE_XDOTOOL_GETFOCUSEDWINDOW_H
