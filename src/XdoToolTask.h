#ifndef NODE_XDOTOOL_TASK_H_
#define NODE_XDOTOOL_TASK_H_

#include "xdo_c.h"
#include "XTask.h"

struct XdoToolTask : XTask {
    explicit XdoToolTask(xdo_t*);
    [[nodiscard]] xdo_t* GetXdo() const;
protected:
    xdo_t* xdo;
};

#endif // NODE_XDOTOOL_TASK_H_
