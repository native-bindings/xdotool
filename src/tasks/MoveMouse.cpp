#include "MoveMouse.h"

using v8::Local;
using v8::Value;

XdoToolTask_MoveMouse::XdoToolTask_MoveMouse(
    xdo_t* xdo,
    int x,
    int y,
    int screen
): XdoToolTask(xdo), x(x), y(y), screen(screen) {}

Local<Value> XdoToolTask_MoveMouse::GetResult() {
    return Nan::Undefined();
}

void XdoToolTask_MoveMouse::Execute() {
    if(xdo_move_mouse(xdo, x, y, screen) != XDO_SUCCESS) {
        SetFailure("Failed to move mouse");
    }
}