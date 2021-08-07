#ifndef NODE_XDOTOOL_TASK_SEND_KEYSEQUENCE_H_
#define NODE_XDOTOOL_TASK_SEND_KEYSEQUENCE_H_

#include "../XdoToolTask.h"

class XdoToolTask_SendKeysequence : public XdoToolTask {
public:
    XdoToolTask_SendKeysequence(xdo_t*, Window, std::string, uint32_t);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    Window window;
    std::string sequence;
    uint32_t delay;
};

#endif // NODE_XDOTOOL_TASK_SEND_KEYSEQUENCE_H_
