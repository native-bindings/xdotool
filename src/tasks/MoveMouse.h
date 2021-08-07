#ifndef NODE_XDOTOOL_MOVE_MOUSE_TASK_H_
#define NODE_XDOTOOL_MOVE_MOUSE_TASK_H_

#include "../XdoToolTask.h"

class XdoToolTask_MoveMouse : public XdoToolTask {
public:
    XdoToolTask_MoveMouse(xdo_t*, int, int, int);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    int x;
    int y;
    int screen;
};

#endif // NODE_XDOTOOL_MOVE_MOUSE_TASK_H_