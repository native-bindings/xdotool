//
// Created by victor on 7/22/21.
//

#ifndef NODE_XDOTOOL_SYNC_H
#define NODE_XDOTOOL_SYNC_H


#include "../XTask.h"

class XdoToolTask_Sync : public XTask {
public:
    explicit XdoToolTask_Sync(Display*);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
};


#endif //NODE_XDOTOOL_SYNC_H
