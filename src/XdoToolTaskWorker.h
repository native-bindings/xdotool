#ifndef NODE_XDOTOOL_TASK_WORKER_H_
#define NODE_XDOTOOL_TASK_WORKER_H_

#include "XTask.h"

class XdoToolTaskWorker : public Nan::AsyncWorker {
public:
    XdoToolTaskWorker(Nan::Callback*, XTask*);
    void Execute() override;
    void HandleOKCallback() override;
private:
    XTask* task;
};

#endif // NODE_XDOTOOL_TASK_WORKER_H_
