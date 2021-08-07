#include "EnterText.h"

using v8::Local;
using v8::Value;

XdoToolTask_EnterText::XdoToolTask_EnterText(
    xdo_t* xdo,
    Window w,
    std::string& text,
    useconds_t delay
):
    XdoToolTask(xdo),
    window(w),
    text(text),
    delay(delay),
    activeMods(nullptr),
    activeModCount(0)
{
}

void XdoToolTask_EnterText::Execute() {
    if(activeMods){
        freeActiveMods();
    }
    if(xdo_get_active_modifiers(xdo, &activeMods, &activeModCount) != XDO_SUCCESS){
        SetFailure("Failed to get active modifiers");
        return;
    }
    if(xdo_clear_active_modifiers(xdo, window, activeMods, activeModCount) != XDO_SUCCESS) {
        SetFailure("Failed to clear active modifiers");
        return;
    }
    if(xdo_enter_text_window(xdo,window,text.c_str(), delay) != XDO_SUCCESS) {
        SetFailure("Failure to enter text");
    }
    freeActiveMods();
}

Local<Value> XdoToolTask_EnterText::GetResult() {
    return Nan::Undefined();
}

void XdoToolTask_EnterText::freeActiveMods() {
    if(activeMods) {
        free(activeMods);
        activeMods = nullptr;
    }
}

XdoToolTask_EnterText::~XdoToolTask_EnterText() {
    freeActiveMods();
}
