#ifndef NODE_XDOTOOL_GETWINDOWSIZE_H
#define NODE_XDOTOOL_GETWINDOWSIZE_H

#include "../XdoToolTask.h"

namespace tasks {

class GetWindowSize : public XdoToolTask {
public:
    GetWindowSize(xdo_t*,XID);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    XID id;
    int status = XDO_SUCCESS;
    unsigned int width, height;
};

}


#endif //NODE_XDOTOOL_GETWINDOWSIZE_H
