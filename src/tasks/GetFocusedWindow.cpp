#include "GetFocusedWindow.h"


XdoToolTask_GetFocusedWindow::XdoToolTask_GetFocusedWindow(xdo_t* xdo):
    XdoToolTask(xdo),
    result(0)
{}

void XdoToolTask_GetFocusedWindow::Execute() {
    if(xdo_get_focused_window_sane(xdo,&result) != XDO_SUCCESS) {
        SetFailure("Failed to get focused window");
    }
}

v8::Local<v8::Value> XdoToolTask_GetFocusedWindow::GetResult() {
    return Nan::New<v8::Number>(result);
}



