#include "GetWindowProperty.h"

XdoToolTask_GetWindowProperty::XdoToolTask_GetWindowProperty(
    xdo_t* xdo,
    Window w,
    const char* property
): XdoToolTask(xdo), property(property), window(w) {

}

void XdoToolTask_GetWindowProperty::Execute() {
    int status = xdo_get_window_property(
        xdo,
        window,
        property,
        &value,
        &nitems,
        &atom,
        &size
    );
    if(status == XDO_SUCCESS) {
        AddResource(value);
    }
}