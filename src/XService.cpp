#include "XService.h"

using v8::String;
using v8::Isolate;

using Nan::New;

XService::XService(Display* display): display(display) {}

Display* XService::GetDisplay() {
    return display;
}
