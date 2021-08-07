//
// Created by victor on 7/22/21.
//

#include "Sync.h"

void XdoToolTask_Sync::Execute() {
    XSync(display,0);
}

v8::Local<v8::Value> XdoToolTask_Sync::GetResult() {
    return Nan::Undefined();
}

XdoToolTask_Sync::XdoToolTask_Sync(Display* display) : XTask(display){

}
