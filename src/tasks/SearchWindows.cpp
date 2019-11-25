#include "SearchWindows.h"

XdoToolTask_SearchWindows::XdoToolTask_SearchWindows(xdo_t* xdo, xdo_search_t* search): XdoToolTask(xdo), search(search) {}

XdoToolTask_SearchWindows::~XdoToolTask_SearchWindows() {
}

void XdoToolTask_SearchWindows::Execute() {
    if(xdo_search_windows(xdo, search, &windows_list, &windows_length) != XDO_SUCCESS) {
        SetFailure("Search windows failed");
    }
}

Local<Value> XdoToolTask_SearchWindows::GetResult() {
    Local<Array> result = New<Array>(windows_length);
    for(unsigned int i = 0; i < windows_length; i++) {
        result->Set(i, New<String>(std::to_string(windows_list[i])).ToLocalChecked());
    }
    return result;
}