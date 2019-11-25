#ifndef NODE_XDOTOOL_GET_WINDOW_PID_TASK_H_
#define NODE_XDOTOOL_GET_WINDOW_PID_TASK_H_

#include "GetWindowProperty.h"

class XdoToolTask_GetWindowPID : public XdoToolTask_GetWindowProperty {
public:
    XdoToolTask_GetWindowPID(xdo_t*, Window);
    Local<Value> GetResult() override;
};

#endif // NODE_XDOTOOL_GET_WINDOW_PID_TASK_H_