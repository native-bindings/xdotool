#ifndef NODE_XDOTOOL_XTASK_H_
#define NODE_XDOTOOL_XTASK_H_

#include "ResourceManager.h"
#include "xdo_c.h"

#include <iostream>

/**
 * Describe X11 task
 */
struct XTask : ResourceManager {
    Display* display;
    std::string failure;
    explicit XTask(Display*);
    ~XTask() override = default;
    void SetFailure(std::string failure);
    virtual void Execute() = 0;
    virtual v8::Local<v8::Value> GetResult() = 0;
};

#endif // NODE_XDOTOOL_XTASK_H_
