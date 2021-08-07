#ifndef NODE_XDOTOOL_WINDOW_HAS_PROPERTY_TASK_H_
#define NODE_XDOTOOL_WINDOW_HAS_PROPERTY_TASK_H_

#include "GetWindowProperty.h"

class XdoToolTask_WindowHasProperty : public XdoToolTask_GetWindowProperty {
public:
    XdoToolTask_WindowHasProperty(xdo_t*, Window, std::string);
    v8::Local<v8::Value> GetResult() override;
private:
    Window window;
};

#endif // NODE_XDOTOOL_WINDOW_HAS_PROPERTY_TASK_H_