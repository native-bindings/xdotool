#include "XTask.h"

XTask::XTask(Display* display): display(display), failure(nullptr) {}

void XTask::SetFailure(const char* failure) {
    if(this->failure != nullptr) {
        std::cerr << "Trying to redefine failure message to: " << failure << std::endl;
        return;
    }
    this->failure = failure;
}

Display* XTask::GetDisplay() {
    return display;
}