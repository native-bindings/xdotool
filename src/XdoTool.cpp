#include "XdoTool.h"
#include "tasks/SearchWindows.h"
#include "tasks/GetMouseLocation.h"
#include "tasks/GetViewportDimensions.h"
#include "tasks/SendKeysequence.h"
#include "tasks/GetWindowPID.h"
#include "tasks/MoveMouse.h"
#include "tasks/ActivateWindow.h"
#include "tasks/EnterText.h"
#include "tasks/WindowHasProperty.h"
#include "ClassCreator.h"
#include "TypeConverter.h"
#include "XdoToolTaskWorker.h"
#include "tasks/Sync.h"
#include "tasks/GetFocusedWindow.h"

#include <iostream>

using v8::FunctionTemplate;
using v8::Function;
using v8::String;
using v8::Local;
using v8::Isolate;
using v8::Object;
using v8::Value;
using Nan::Set;
using Nan::To;
using Nan::GetCurrentContext;
using Nan::Callback;
using Nan::New;
using Nan::FunctionCallback;

Nan::Persistent<Function> XdoTool::constructor;

XdoTool::XdoTool(xdo_t* xdo): XService(xdo->xdpy), xdo(xdo) {
}

XdoTool::~XdoTool() {
    xdo_free(xdo);
}

NAN_METHOD(XdoTool::GetViewportDimensions) {
    int screen;
    if(!TypeConverter::GetInt32(info[0], screen)){
        Nan::ThrowError("First argument must be a valid integer");
        return;
    }
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_GetViewportDimensions(tool->xdo, screen);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::SendKeysequence) {
    XdoTool* tool = nullptr;
    char* keySequence = nullptr;
    Window window;
    int32_t delay;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    if(!TypeConverter::GetWindow(info[0],window)) {
        Nan::ThrowError("First argument must be a valid window identifier");
        return;
    }
    if(!TypeConverter::GetString(info[1], &keySequence)) {
        Nan::ThrowError("Second parameter must be a valid string");
        return;
    }
    if(!TypeConverter::GetInt32(info[2], delay)) {
        Nan::ThrowError("Third parameter must be a valid integer");
        return;
    }
    auto callback = new Callback(To<Function>(info[3]).ToLocalChecked());
    auto task = new XdoToolTask_SendKeysequence(tool->xdo, window, keySequence, delay);
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::GetWindowPID) {
    Window window;
    if(!TypeConverter::GetWindow(info[0],window)) {
        Nan::ThrowError("First argument must be a window");
        return;
    }
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_GetWindowPID(tool->xdo, window);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::MoveMouse) {
    if(info[0]->IsUndefined() || !info[0]->IsObject()) {
        Nan::ThrowError("First argument must be a valid object");
        return;
    }

    int x, y, screen = 0;

    Local<Object> opts = info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
    if(!TypeConverter::GetInt32(opts, "x", &x) || !TypeConverter::GetInt32(opts, "y", &y)) {
        Nan::ThrowError("x and y properties must be defined");
        return;
    }
    TypeConverter::GetInt32(opts, "screen_num", &screen);

    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_MoveMouse(tool->xdo, x, y, screen);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::WindowHasProperty) {
    Window window;
    if(!TypeConverter::GetWindow(info[0],window)) {
        Nan::ThrowError("First argument must be a valid window");
        return;
    }
    std::string property;
    if(!TypeConverter::GetString(info[1], property)) {
        Nan::ThrowError("Second argument must be a valid string representing window property");
        return;
    }
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_WindowHasProperty(tool->xdo, window, property);
    auto callback = new Callback(To<Function>(info[2]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::ActivateWindow) {
    Window window;
    if(!TypeConverter::GetWindow(info[0],window)) {
        Nan::ThrowError("First argument must be a valid string pointing to a Window object");
        return;
    }
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_ActivateWindow(tool->xdo, window);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::EnterText) {
    Window window;
    std::string text;
    int32_t delay;
    if(!TypeConverter::GetWindow(info[0],window)) {
        Nan::ThrowError("First argument must be a valid string pointing to a Window object");
        return;
    }
    if(!TypeConverter::GetString(info[1],text)) {
        Nan::ThrowError("Second argument must be a string");
        return;
    }
    if(!TypeConverter::GetInt32(info[2],delay)) {
        Nan::ThrowError("Third argument must be an integer");
        return;
    }
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_EnterText(tool->xdo, window, text, delay);
    auto callback = new Callback(To<Function>(info[3]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::GetFocusedWindow) {
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_GetFocusedWindow(tool->xdo);
    auto callback = new Callback(To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback,task));
}

void XdoTool::Init(Local<Object> exports) {
    std::map<std::string, FunctionCallback> methods {
        { "constructor", Constructor },
        { "getMouseLocation", GetMouseLocation },
        { "searchWindows", SearchWindows },
        { "getWindowPID", GetWindowPID },
        { "moveMouse", MoveMouse },
        { "getFocusedWindow", GetFocusedWindow },
        { "enterText", EnterText },
        { "sync", Sync },
        { "sendKeysequence", SendKeysequence },
        { "windowHasProperty", WindowHasProperty },
        { "activateWindow", ActivateWindow },
        { "getViewportDimensions", GetViewportDimensions }
    };

    Set(
        exports,
        Nan::New<String>("XdoTool").ToLocalChecked(),
        ClassCreator::NewClass<XdoTool>("XdoTool", methods)
    );
}

NAN_METHOD(XdoTool::Constructor) {
    xdo_t* xdo = xdo_new(nullptr);
    if(xdo == nullptr) {
        Nan::ThrowError("Failed to create xdo_t instance. Please make sure X.org is running");
        return;
    }

    auto* tool = new XdoTool(xdo);
    printf("%p\n",tool);
    tool->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(XdoTool::SearchWindows) {
    if(!info[0]->IsObject()) {
        Nan::ThrowError("First argument must be a valid object");
        return;
    }

    auto search = (xdo_search_t*) calloc(1, sizeof(xdo_search_t)*1);
    if(search == nullptr) {
        Nan::ThrowError("Memory allocation failed");
        return;
    }
    search->require = xdo_search_t::SEARCH_ANY;
    search->max_depth = -1;

    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_SearchWindows(tool->xdo, search);
    Local<Object> search_obj = Nan::To<Object>(info[0]).ToLocalChecked();

    task->AddResource(search);

    char* string;
    if(TypeConverter::GetString(search_obj, "title", &string)) {
        search->title = string;
        search->searchmask |= SEARCH_TITLE;
        task->AddResource(string);
    }

    if(TypeConverter::GetString(search_obj, "winclass", &string)) {
        search->winclass = string;
        search->searchmask |= SEARCH_CLASS;
        task->AddResource(string);
    }

    if(TypeConverter::GetString(search_obj, "winclassname", &string)) {
        search->winclassname = string;
        search->searchmask |= SEARCH_CLASSNAME;
        task->AddResource(string);
    }

    if(TypeConverter::GetString(search_obj, "winname", &string)) {
        search->winname = string;
        search->searchmask |= SEARCH_NAME;
    }

    int32_t int32_value;
    if(TypeConverter::GetInt32(search_obj, "pid", &int32_value)) {
        search->pid = int32_value;
    }

    if(TypeConverter::GetInt32(search_obj, "maxDepth", &int32_value)) {
        search->max_depth = int32_value;
    }

    bool boolean_value;
    if(TypeConverter::GetBool(search_obj, "onlyVisible", &boolean_value)) {
        search->only_visible = boolean_value ? 1 : 0;
    }

    if(TypeConverter::GetInt32(search_obj, "screen", &int32_value)) {
        search->screen = int32_value;
    }

    if(TypeConverter::GetInt32(search_obj, "desktop", &int32_value)) {
        search->desktop = int32_value;
    }

    if(TypeConverter::GetInt32(search_obj, "limit", &int32_value)) {
        search->limit = int32_value;
    }

    auto callback = new Nan::Callback(Nan::To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::GetMouseLocation) {
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_GetMouseLocation(tool->xdo);
    auto callback = new Nan::Callback(Nan::To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::Sync) {
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(),&tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    auto task = new XdoToolTask_Sync(tool->xdo->xdpy);
    auto callback = new Nan::Callback(Nan::To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}
