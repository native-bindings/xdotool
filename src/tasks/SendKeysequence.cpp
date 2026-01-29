#include "SendKeysequence.h"

#include <utility>

using v8::Local;
using v8::Value;

XdoToolTask_SendKeysequence::XdoToolTask_SendKeysequence(
    xdo_t* xdo,
    const Window window,
    std::string sequence,
    const uint32_t delay
): XdoToolTask(xdo), window(window), sequence(std::move(sequence)), delay(delay) {

}

void XdoToolTask_SendKeysequence::Execute() {
    bool clear_modifiers = false;
    int active_mods_n = 0;
    charcodemap_t* active_mods = nullptr;
    if (clear_modifiers) {
        if(xdo_get_active_modifiers(xdo, &active_mods, &active_mods_n) != XDO_SUCCESS) {
            SetFailure("Failed to get active modifiers");
            return;
        }
        if(xdo_clear_active_modifiers(xdo, window, active_mods, active_mods_n) != XDO_SUCCESS) {
            SetFailure("Failed to clear active modifiers");
            return;
        }
    }
    if(xdo_send_keysequence_window(xdo, window, sequence.c_str(), delay) != XDO_SUCCESS) {
        SetFailure("Failed to send key sequence to window");
    }
    if(clear_modifiers) {
        if(xdo_set_active_modifiers(xdo, window, active_mods, active_mods_n) != XDO_SUCCESS) {
            SetFailure("Failed to set active modifiers");
        }
        free(active_mods);
    }
}

Local<Value> XdoToolTask_SendKeysequence::GetResult() {
    return Nan::Undefined();
}