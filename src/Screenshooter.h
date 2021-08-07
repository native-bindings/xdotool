#ifndef NODE_XDOTOOL_SCREENSHOOTER_H_
#define NODE_XDOTOOL_SCREENSHOOTER_H_

#include "XService.h"

#include <boost/optional.hpp>

class XScreenshooter : public XService {
public:
    XScreenshooter(Display*, Window);
    ~XScreenshooter();
    v8::Local<v8::ArrayBuffer> GetArrayBuffer();
    void GetImage();
    uint32_t ByteLength();
    uint8_t* Data();
    static void Init(v8::Local<v8::Object>);
    static Nan::Persistent<v8::Function> constructor;
private:
    Nan::Persistent<v8::ArrayBuffer> arrayBuffer;
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