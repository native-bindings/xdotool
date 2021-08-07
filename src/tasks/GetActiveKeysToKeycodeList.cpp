//
// Created by victor on 7/22/21.
//

#include "GetActiveKeysToKeycodeList.h"

using v8::Local;
using v8::Value;

void GetActiveKeysToKeycodeList::Execute() {
    if(xdo_get_active_keys_to_keycode_list(xdo,&keys,&key_count) !=XDO_SUCCESS){
        SetFailure("Failed to get active keys to keycode list");
    }
}

GetActiveKeysToKeycodeList::GetActiveKeysToKeycodeList(XKeyboard* kbd):
    XTask(kbd->display),
    kbd(kbd)
{

}

Local<Value> GetActiveKeysToKeycodeList::GetResult() {
    return Nan::Null();
}
