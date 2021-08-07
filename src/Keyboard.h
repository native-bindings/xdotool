#ifndef NODE_XDOTOOL_KEYBOARD_H_
#define NODE_XDOTOOL_KEYBOARD_H_

#include "XService.h"

class XKeyboard : public XService {
public:
    char keys[32];
    explicit XKeyboard(Display*);
    static void Init(v8::Local<v8::Object>);

    Nan::Persistent<v8::Object> arrayBuffer;
    v8::Local<v8::Value> GetBuffer();
    ~XKeyboard() override;
    static Nan::Persistent<v8::Function> constructor;
private:
    static NAN_METHOD(QueryKeymap);
    static NAN_METHOD(Constructor);
    static NAN_METHOD(KeycodeToKeysym);
    static NAN_METHOD(KeysymToString);
    static NAN_METHOD(GetActiveKeysToKeycodeList);
};

#endif // NODE_XDOTOOL_KEYBOARD_H_