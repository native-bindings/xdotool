#include "XTask.h"

XTask::XTask(Display* display): display(display), failure("") {}

void XTask::SetFailure(std::string newFailure) {
    if(!failure.empty()) {
        std::cerr << "Trying to redefine failure message to: " << newFailure << std::endl;
        return;
    }
    failure = newFailure;
}
