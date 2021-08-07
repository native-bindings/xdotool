#ifndef NODE_XDOTOOL_GET_IMAGE_TASK_H_
#define NODE_XDOTOOL_GET_IMAGE_TASK_H_

#include "../Screenshooter.h"
#include "../XTask.h"

class XdoToolTask_GetImage : public XTask {
public:
    explicit XdoToolTask_GetImage(XScreenshooter*);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    XScreenshooter* screenshooter;
};

#endif // NODE_XDOTOOL_GET_IMAGE_TASK_H_