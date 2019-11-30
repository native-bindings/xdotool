#ifndef NODE_XDOTOOL_X_SERVICE_H_
#define NODE_XDOTOOL_X_SERVICE_H_

#include "ResourceManager.h"
#include "xdo_c.h"

#include <map>

using v8::Local;
using v8::Function;
using v8::FunctionTemplate;
using v8::String;
using v8::Isolate;

using Nan::New;
using Nan::Persistent;
using Nan::FunctionCallback;

class XService : public Nan::ObjectWrap, public ResourceManager {
public:
    XService(Display*);
    Display* GetDisplay();
protected:
    Display* display;
};

#endif // NODE_XDOTOOL_X_SERVICE_H_