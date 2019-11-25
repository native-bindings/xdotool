#ifndef NODE_XDOTOOL_GET_MOUSE_LOCATION_H_
#define NODE_XDOTOOL_GET_MOUSE_LOCATION_H_

#include "../XdoToolTask.h"

using v8::Object;
using v8::String;
using v8::Number;
using Nan::New;
using Nan::Set;

class XdoToolTask_GetMouseLocation : public XdoToolTask {
public:
    XdoToolTask_GetMouseLocation(xdo_t*);
    ~XdoToolTask_GetMouseLocation() override;
    void Execute() override;
    Local<Value> GetResult() override;
private:
    int x;
    int y;
    int screen_num;
};

#endif // NODE_XDOTOOL_GET_MOUSE_LOCATION_H_
