#include "XdoToolTaskWorker.h"

using v8::Local;
using v8::Value;

XdoToolTaskWorker::XdoToolTaskWorker(Nan::Callback* callback, XTask* task): Nan::AsyncWorker(callback), task(task) {
}

void XdoToolTaskWorker::Execute() {
    XLockDisplay(task->display);
    task->Execute();
    XUnlockDisplay(task->display);
}

void XdoToolTaskWorker::HandleOKCallback() {
    std::vector<Local<Value>> argv(2);
    if(!task->failure.empty()) {
        argv[0] = Nan::Error(task->failure.c_str());
        argv[1] = Nan::Undefined();
    } else {
        argv[0] = Nan::Undefined();
        argv[1] = task->GetResult();
    }
    callback->Call(argv.size(), argv.data(), async_resource);
    delete task;
}
