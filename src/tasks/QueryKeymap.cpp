#include "QueryKeymap.h"

#include <X11/XKBlib.h>

using v8::Local;
using v8::Value;

XTask_QueryKeymap::XTask_QueryKeymap(XKeyboard* keyboard): XTask(keyboard->display), keyboard(keyboard) {
}

void XTask_QueryKeymap::Execute() {
    if(XQueryKeymap(display, keyboard->keys) != X_OK) {
        SetFailure("Failed to query keymap");
        return;
    }
}

Local<Value> XTask_QueryKeymap::GetResult() {
    return keyboard->GetBuffer();
}