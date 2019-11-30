#include "XdoToolTaskWorker.h"

XdoToolTaskWorker::XdoToolTaskWorker(Nan::Callback* callback, XTask* task): Nan::AsyncWorker(callback), task(task) {
}

void XdoToolTaskWorker::Execute() {
    XLockDisplay(task->GetDisplay());
    task->Execute();
    XUnlockDisplay(task->GetDisplay());
}

void XdoToolTaskWorker::HandleOKCallback() {
    std::vector<Local<Value>> argv(2);
    if(task->failure != nullptr) {
        argv[0] = Nan::Error(task->failure);
        argv[1] = Nan::Undefined();
    } else {
        argv[0] = Nan::Undefined();
        argv[1] = task->GetResult();
    }
    callback->Call(argv.size(), argv.data(), async_resource);
    delete task;
}
