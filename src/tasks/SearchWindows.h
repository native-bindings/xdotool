#ifndef NODE_XDOTOOL_TASK_SEARCH_WINDOWS_H_
#define NODE_XDOTOOL_TASK_SEARCH_WINDOWS_H_

#include "../XdoToolTask.h"

using v8::Array;
using v8::String;
using Nan::New;

class XdoToolTask_SearchWindows : public XdoToolTask {
public:
    XdoToolTask_SearchWindows(xdo_t* xdo, xdo_search_t* search);
    ~XdoToolTask_SearchWindows() override;
    void Execute() override;
    Local<Value> GetResult() override;
private:
    xdo_search_t* search;
    Window* windows_list;
    unsigned int windows_length;
};

#endif // NODE_XDOTOOL_TASK_SEARCH_WINDOWS_H_
