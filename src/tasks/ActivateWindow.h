#ifndef NODE_XDOTOOL_ACTIVATE_WINDOW_TASK_H_
#define NODE_XDOTOOL_ACTIVATE_WINDOW_TASK_H_

#include "../XdoToolTask.h"

namespace tasks {

class ActivateWindow : public XdoToolTask {
public:
    ActivateWindow(xdo_t*, Window);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    Window window;
};

}

#endif // NODE_XDOTOOL_ACTIVATE_WINDOW_TASK_H_