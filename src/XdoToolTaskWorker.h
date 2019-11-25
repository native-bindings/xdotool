#ifndef NODE_XDOTOOL_TASK_WORKER_H_
#define NODE_XDOTOOL_TASK_WORKER_H_

#include "XdoToolTask.h"

class XdoToolTaskWorker : public Nan::AsyncWorker {
public:
    XdoToolTaskWorker(Nan::Callback*, XdoToolTask*);
    void Execute() override;
    void HandleOKCallback() override;
private:
    XdoToolTask* task;
};

#endif // NODE_XDOTOOL_TASK_WORKER_H_
