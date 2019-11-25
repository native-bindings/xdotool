#include "XdoTool.h"
#include "tasks/SearchWindows.h"
#include "tasks/GetMouseLocation.h"
#include "tasks/GetViewportDimensions.h"
#include "tasks/GetWindowPID.h"
#include "tasks/MoveMouse.h"
#include "tasks/ActivateWindow.h"
#include "tasks/WindowHasProperty.h"
#include "XdoToolTaskWorker.h"

#include <iostream>

using v8::FunctionTemplate;
using v8::Function;
using v8::String;
using v8::Isolate;
using v8::Value;
using Nan::Set;
using Nan::To;
using Nan::Callback;
using Nan::New;

XdoTool::XdoTool(xdo_t* xdo): Nan::ObjectWrap(), xdo(xdo) {
}

XdoTool::~XdoTool() {
    xdo_free(xdo);
}

NAN_METHOD(XdoTool::GetViewportDimensions) {
    if(!info[0]->IsNumber()) {
        Nan::ThrowError("First argument must be a valid number");
        return;
    }

    int screen;
    if(!info[0]->Int32Value(Nan::GetCurrentContext()).To(&screen)) {
        std::cerr << "Empty value for screen" << std::endl;
    }
    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_GetViewportDimensions(tool->xdo, screen);
    auto callback = new Nan::Callback(Nan::To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

Window GetWindow(Local<Value> value) {
    if(value->IsUndefined() || !value->IsString()) {
        return 0;
    }

    Local<String> window_str = value->ToString(Nan::GetCurrentContext()).ToLocalChecked();
    char* window_str_value = new char[window_str->Utf8Length(Nan::GetCurrentContext()->GetIsolate())];
    window_str->WriteUtf8(Nan::GetCurrentContext()->GetIsolate(), window_str_value);
    Window window = (Window) atol(window_str_value);
    delete window_str_value;

    return window;  
}

NAN_METHOD(XdoTool::GetWindowPID) {
    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto window = GetWindow(info[0]);
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
    if(!GetInt32(opts, "x", &x) || !GetInt32(opts, "y", &y)) {
        Nan::ThrowError("x and y properties must be defined");
        return;
    }
    GetInt32(opts, "screen_num", &screen);

    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_MoveMouse(tool->xdo, x, y, screen);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::WindowHasProperty) {
    Window window = GetWindow(info[0]);
    if(!window) {
        Nan::ThrowError("First argument must be a valid window");
        return;
    }
    char* property;
    if(!GetString(info[1], &property)) {
        Nan::ThrowError("Second argument must be a valid string representing window property");
        return;
    }
    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_WindowHasProperty(tool->xdo, window, property);
    task->AddResource(property);
    auto callback = new Callback(To<Function>(info[2]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::ActivateWindow) {
    Window window = GetWindow(info[0]);
    if(!window) {
        Nan::ThrowError("First argument must be a valid string pointing to a Window object");
        return;
    }
    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_ActivateWindow(tool->xdo, window);
    auto callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

void XdoTool::Init(Local<Object> exports) {
    std::vector<std::pair<const char*, Nan::FunctionCallback>> methods {
        { "getMouseLocation", GetMouseLocation },
        { "searchWindows", SearchWindows },
        { "getWindowPID", GetWindowPID },
        { "moveMouse", MoveMouse },
        { "windowHasProperty", WindowHasProperty },
        { "activateWindow", ActivateWindow },
        { "getViewportDimensions", GetViewportDimensions }
    };

    Local<FunctionTemplate> tpl = New<FunctionTemplate>(Constructor);
    tpl->SetClassName(New<String>("XdoTool").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(methods.size());

    for(auto& method : methods) {
        Nan::SetPrototypeMethod(tpl, method.first, method.second);
    }

    Set(exports, Nan::New<String>("XdoTool").ToLocalChecked(), tpl->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
}

NAN_METHOD(XdoTool::Constructor) {
    xdo_t* xdo = xdo_new(nullptr);
    if(xdo == nullptr) {
        Nan::ThrowError("Failed to create xdo_t instance. Please make sure X.org is running");
    }

    XdoTool* tool = new XdoTool(xdo);
    tool->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

bool XdoTool::GetString(Local<Value> value, char** out) {
    if(value->IsUndefined() || !value->IsString()) {
        return false;
    }
    Isolate* isolate = Nan::GetCurrentContext()->GetIsolate();
    Local<String> title_string = Nan::To<String>(value).ToLocalChecked();
    int length = title_string->Utf8Length(isolate);
    auto buffer = (char*)malloc(sizeof(char)*(length + 1));
    if(buffer == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return false;
    }
    title_string->WriteUtf8(isolate, buffer);
    buffer[length] = 0;
    *out = buffer;
    return true;
}

/**
 * @returns true if string was found and allocated, false otherwise
 */
bool XdoTool::GetString(Local<Object> obj, const char* prop, char** out) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    return GetString(value, out);
}

bool XdoTool::GetInt32(Local<Object> obj, const char* prop, int32_t* out_n) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsInt32()) {
        return false;
    }
    if(!value->Int32Value(Nan::GetCurrentContext()).To(out_n)) {
        std::cerr << "Received empty value for property name: " << prop << std::endl;
    }
    return true;
}

bool XdoTool::GetBool(Local<Object> obj, const char* prop, bool* out_bool) {
    Local<Value> value = Nan::Get(obj, Nan::New<String>(prop).ToLocalChecked()).ToLocalChecked();
    if(value->IsUndefined() || !value->IsBoolean()) {
        return false;
    }
    *out_bool = value->BooleanValue(Nan::GetCurrentContext()->GetIsolate());
    return true;
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

    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_SearchWindows(tool->xdo, search);
    Local<Object> search_obj = Nan::To<Object>(info[0]).ToLocalChecked();

    task->AddResource(search);

    char* string;
    if(GetString(search_obj, "title", &string)) {
        search->title = string;
        search->searchmask |= SEARCH_TITLE;
        task->AddResource(string);
    }

    if(GetString(search_obj, "winclass", &string)) {
        search->winclass = string;
        search->searchmask |= SEARCH_CLASS;
        task->AddResource(string);
    }

    if(GetString(search_obj, "winclassname", &string)) {
        search->winclassname = string;
        search->searchmask |= SEARCH_CLASSNAME;
        task->AddResource(string);
    }

    if(GetString(search_obj, "winname", &string)) {
        search->winname = string;
        search->searchmask |= SEARCH_NAME;
    }

    int32_t int32_value;
    if(GetInt32(search_obj, "pid", &int32_value)) {
        search->pid = int32_value;
    }

    if(GetInt32(search_obj, "maxDepth", &int32_value)) {
        search->max_depth = int32_value;
    }

    bool boolean_value;
    if(GetBool(search_obj, "onlyVisible", &boolean_value)) {
        search->only_visible = boolean_value ? 1 : 0;
    }

    if(GetInt32(search_obj, "screen", &int32_value)) {
        search->screen = int32_value;
    }

    if(GetInt32(search_obj, "desktop", &int32_value)) {
        search->desktop = int32_value;
    }

    if(GetInt32(search_obj, "limit", &int32_value)) {
        search->limit = int32_value;
    }

    auto callback = new Nan::Callback(Nan::To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}

NAN_METHOD(XdoTool::GetMouseLocation) {
    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(info.This());
    auto task = new XdoToolTask_GetMouseLocation(tool->xdo);
    auto callback = new Nan::Callback(Nan::To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}