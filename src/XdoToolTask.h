#ifndef NODE_XDOTOOL_TASK_H_
#define NODE_XDOTOOL_TASK_H_

#include "ResourceManager.h"
#include "xdo_c.h"

#include <iostream>

using v8::Local;
using v8::Value;

struct XdoToolTask : public ResourceManager {
public:
    const char* failure;
    XdoToolTask(xdo_t* xdo);
    virtual ~XdoToolTask() {}
    void SetFailure(const char* failure);
    virtual void Execute() = 0;
    virtual Local<Value> GetResult() = 0;
    xdo_t* GetXdo();
protected:
    xdo_t* xdo;
};

#endif // NODE_XDOTOOL_TASK_H_
