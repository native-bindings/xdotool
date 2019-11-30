#ifndef NODE_XDOTOOL_KEYBOARD_H_
#define NODE_XDOTOOL_KEYBOARD_H_

#include "XService.h"

using v8::Object;
using v8::ArrayBuffer;
using v8::Local;

class XKeyboard : public XService {
public:
    XKeyboard(Display*);
    static void Init(Local<Object>);
    static Persistent<Function> constructor;
    Persistent<ArrayBuffer> arrayBuffer;
    char* Keys();
    Local<ArrayBuffer> GetArrayBuffer();
    ~XKeyboard();
private:
    char keys[32];
    static NAN_METHOD(QueryKeymap);
    static NAN_METHOD(Constructor);
    static NAN_METHOD(KeycodeToKeysym);
    static NAN_METHOD(KeysymToString);
};

#endif // NODE_XDOTOOL_KEYBOARD_H_