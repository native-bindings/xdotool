#ifndef NODE_XDOTOOL_GET_MOUSE_LOCATION_H_
#define NODE_XDOTOOL_GET_MOUSE_LOCATION_H_

#include "../XdoToolTask.h"

class XdoToolTask_GetMouseLocation : public XdoToolTask {
public:
    explicit XdoToolTask_GetMouseLocation(xdo_t*);
    ~XdoToolTask_GetMouseLocation() override = default;
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    int x;
    int y;
    int screen_num;
};

#endif // NODE_XDOTOOL_GET_MOUSE_LOCATION_H_
