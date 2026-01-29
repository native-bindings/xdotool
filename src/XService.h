#ifndef NODE_XDOTOOL_X_SERVICE_H_
#define NODE_XDOTOOL_X_SERVICE_H_

#include "ResourceManager.h"
#include "xdo_c.h"

class XService : public Nan::ObjectWrap, public ResourceManager {
public:
    Display* display;
    explicit XService(Display*);
};

#endif // NODE_XDOTOOL_X_SERVICE_H_