#ifndef NODE_XDOTOOL_SCREENSHOOTER_H_
#define NODE_XDOTOOL_SCREENSHOOTER_H_

#include "XService.h"

class XScreenshooter : public XService {
public:
    XScreenshooter(Display*, Window);
    ~XScreenshooter() override;
    v8::Local<v8::Object> GetArrayBuffer();
    void GetImage();
    [[nodiscard]] uint32_t ByteLength() const;
    [[nodiscard]] uint8_t* Data() const;
    static void Init(v8::Local<v8::Object>);
    static Nan::Persistent<v8::Function> constructor;
private:
    Nan::Persistent<v8::Object> persistentBuffer;
    Window window;
    XWindowAttributes attributes{};
    XImage* image = nullptr;
    /**
     * Data where we store raw image data
     */
    uint8_t* data;
    static NAN_METHOD(GetImage);
    static NAN_METHOD(Constructor);
};

#endif // NODE_XDOTOOL_SCREENSHOOTER_H_