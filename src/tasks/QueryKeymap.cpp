#include "QueryKeymap.h"

#include <X11/XKBlib.h>

#define XK_MISCELLANY
#include <X11/keysymdef.h>

XTask_QueryKeymap::XTask_QueryKeymap(XKeyboard* keyboard): XTask(keyboard->GetDisplay()), keyboard(keyboard) {
}

void XTask_QueryKeymap::Execute() {
    if(XQueryKeymap(display, keyboard->Keys()) != X_OK) {
        SetFailure("Failed to query keymap");
        return;
    }
}

Local<Value> XTask_QueryKeymap::GetResult() {
    return keyboard->GetArrayBuffer();
}