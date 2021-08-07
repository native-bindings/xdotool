#ifndef NODE_XDOTOOL_ENTER_TEXT_TASK_H_
#define NODE_XDOTOOL_ENTER_TEXT_TASK_H_

#include "../XdoToolTask.h"

class XdoToolTask_EnterText : public XdoToolTask {
public:
    XdoToolTask_EnterText(xdo_t*, Window, std::string&, useconds_t);
    ~XdoToolTask_EnterText();
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    Window window;
    std::string text;
    useconds_t delay;
    charcodemap_t* activeMods;
    int activeModCount;
    void freeActiveMods();
};

#endif // NODE_XDOTOOL_ENTER_TEXT_TASK_H_