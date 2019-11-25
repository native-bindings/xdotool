#include "XdoToolTask.h"

XdoToolTask::XdoToolTask(xdo_t* xdo): xdo(xdo), failure(nullptr) {}

void XdoToolTask::SetFailure(const char* failure) {
    if(this->failure != nullptr) {
        std::cerr << "Trying to redefine failure message to: " << failure << std::endl;
        return;
    }
    this->failure = failure;
}

xdo_t* XdoToolTask::GetXdo() {
    return xdo;
}