#include "XdoToolTaskWorker.h"

XdoToolTaskWorker::XdoToolTaskWorker(Nan::Callback* callback, XTask* task): Nan::AsyncWorker(callback), task(task) {
}

void XdoToolTaskWorker::Execute() {
    if(task == nullptr) {
        throw std::runtime_error("Task is null");
    }
    XLockDisplay(task->display);
    task->Execute();
    XUnlockDisplay(task->display);
}

void XdoToolTaskWorker::HandleOKCallback() {
    std::vector<v8::Local<v8::Value>> argv(2);
    if(!task->failure.empty()) {
        argv[0] = Nan::Error(task->failure.c_str());
        argv[1] = Nan::Undefined();
    } else {
        argv[0] = Nan::Undefined();
        argv[1] = task->GetResult();
    }
    callback->Call(argv.size(), argv.data(), async_resource);

    delete task;
    task = nullptr;
}
