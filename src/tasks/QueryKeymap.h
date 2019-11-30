#ifndef NODE_XDOTOOL_QUERY_KEYMAP_TASK_H_
#define NODE_XDOTOOL_QUERY_KEYMAP_TASK_H_

#include "../XTask.h"
#include "../Keyboard.h"

class XTask_QueryKeymap : public XTask {
public:
    XTask_QueryKeymap(XKeyboard*);
    void Execute() override;
    Local<Value> GetResult() override;
private:
    XKeyboard* keyboard;
};

#endif // NODE_XDOTOOL_QUERY_KEYMAP_TASK_H_