#include "ActivateWindow.h"

using v8::Local;
using v8::Value;

tasks::ActivateWindow::ActivateWindow(
    xdo_t* xdo,
    Window window
): XdoToolTask(xdo), window(window) {}

void tasks::ActivateWindow::Execute() {
    if(xdo_activate_window(xdo, window) != XDO_SUCCESS) {
        SetFailure("Failed to activate window");
        return;
    }
    if(xdo_wait_for_window_active(xdo, window, 1) != XDO_SUCCESS) {
        SetFailure("Failed to wait for window to be active");
    }
}

Local<Value> tasks::ActivateWindow::GetResult() {
    return Nan::Undefined();
}
