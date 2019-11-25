#ifndef NODE_XDOTOOL_GET_WINDOW_PROPERTY_H_
#define NODE_XDOTOOL_GET_WINDOW_PROPERTY_H_

#include "../XdoToolTask.h"

class XdoToolTask_GetWindowProperty : public XdoToolTask {
public:
    XdoToolTask_GetWindowProperty(xdo_t*, Window, const char*);
    void Execute() override;
private:
    Window window;
    const char* property;
protected:
    unsigned char* value;
    long nitems;
    int size;
    Atom atom;
};

#endif // NODE_XDOTOOL_GET_WINDOW_PROPERTY_H_