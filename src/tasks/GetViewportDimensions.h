#ifndef NODE_XDOTOOL_GET_VIEWPORT_DIMENSIONS_H_
#define NODE_XDOTOOL_GET_VIEWPORT_DIMENSIONS_H_

#include "../XdoToolTask.h"

namespace tasks {

class GetViewportDimensions : public XdoToolTask {
public:
    GetViewportDimensions(xdo_t*, int);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    int screen;
    unsigned int width;
    unsigned int height;
};

}

#endif // NODE_XDOTOOL_GET_VIEWPORT_DIMENSIONS_H_