#ifndef NODE_XDOTOOL_GETACTIVEKEYSTOKEYCODELIST_H
#define NODE_XDOTOOL_GETACTIVEKEYSTOKEYCODELIST_H

#include "../XTask.h"
#include "../Keyboard.h"

class GetActiveKeysToKeycodeList : public XTask {
public:
    explicit GetActiveKeysToKeycodeList(XKeyboard*);
    void Execute() override;
    v8::Local<v8::Value> GetResult() override;
private:
    XKeyboard* kbd;
    xdo_t* xdo;
    charcodemap_t* keys;
    int key_count;
};

#endif //NODE_XDOTOOL_GETACTIVEKEYSTOKEYCODELIST_H
