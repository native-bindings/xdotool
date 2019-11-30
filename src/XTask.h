#ifndef NODE_XDOTOOL_XTASK_H_
#define NODE_XDOTOOL_XTASK_H_

#include "ResourceManager.h"
#include "xdo_c.h"

#include <iostream>

using v8::Local;
using v8::Value;

/**
 * Describe X11 task
 */
struct XTask : public ResourceManager {
public:
    const char* failure;
    XTask(Display*);
    virtual ~XTask() {}
    void SetFailure(const char* failure);
    virtual void Execute() = 0;
    virtual Local<Value> GetResult() = 0;
    Display* GetDisplay();
protected:
    Display* display;
};

#endif // NODE_XDOTOOL_XTASK_H_
