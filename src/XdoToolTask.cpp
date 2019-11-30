#include "XdoToolTask.h"

XdoToolTask::XdoToolTask(xdo_t* xdo): XTask(xdo->xdpy), xdo(xdo) {}

xdo_t* XdoToolTask::GetXdo() {
    return xdo;
}