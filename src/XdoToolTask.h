#ifndef NODE_XDOTOOL_TASK_H_
#define NODE_XDOTOOL_TASK_H_

#include "ResourceManager.h"
#include "xdo_c.h"
#include "XTask.h"

#include <iostream>

struct XdoToolTask : public XTask {
public:
    XdoToolTask(xdo_t*);
    xdo_t* GetXdo();
protected:
    xdo_t* xdo;
};

#endif // NODE_XDOTOOL_TASK_H_
