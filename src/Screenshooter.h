#ifndef NODE_XDOTOOL_SCREENSHOOTER_H_
#define NODE_XDOTOOL_SCREENSHOOTER_H_

#include "XService.h"

#include <boost/optional.hpp>

using v8::Local;
using v8::Object;
using v8::Function;
using v8::ArrayBuffer;
using Nan::Persistent;

class XScreenshooter : public XService {
public:
    XScreenshooter(Display*, Window);
    ~XScreenshooter();
    Local<ArrayBuffer> GetArrayBuffer();
    void GetImage();
    uint32_t ByteLength();
    uint8_t* Data();
    static void Init(Local<Object>);
    static Persistent<Function> constructor;
private:
    Persistent<ArrayBuffer> arrayBuffer;
    Window window;
    XWindowAttributes attributes;
    XImage* image;
    /**
     * Data where we store raw image data
     */
    uint8_t* data;
    static NAN_METHOD(GetImage);
    static NAN_METHOD(Constructor);
};

#endif // NODE_XDOTOOL_SCREENSHOOTER_H_