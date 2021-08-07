#include "GetWindowProperty.h"

XdoToolTask_GetWindowProperty::XdoToolTask_GetWindowProperty(
    xdo_t* xdo,
    Window w,
    std::string property
): XdoToolTask(xdo), property(property), window(w) {

}

void XdoToolTask_GetWindowProperty::Execute() {
    int status = xdo_get_window_property(
        xdo,
        window,
        property.c_str(),
        &value,
        &nitems,
        &atom,
        &size
    );
    if(status == XDO_SUCCESS) {
        AddResource(value);
    }
}