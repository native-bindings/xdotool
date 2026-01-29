#include "Screenshooter.h"
#include "TypeConverter.h"
#include "XdoToolTaskWorker.h"
#include "tasks/GetImage.h"
#include "XdoTool.h"
#include "ClassCreator.h"

#include <X11/Xutil.h>

Nan::Persistent<v8::Function> XScreenshooter::constructor;

XScreenshooter::XScreenshooter(Display* display, const Window window): XService(display), window(window) {
    if(XGetWindowAttributes(display, window, &attributes) != X_OK) {
        throw std::runtime_error("Failed to get window attributes");
    }
    data = static_cast<uint8_t*>(malloc(attributes.width * attributes.height * 3));
    if(data == nullptr) {
        throw std::runtime_error("Memory allocation failed");
    }
    AddResource(data);
}

XScreenshooter::~XScreenshooter() {
    printf("Destroying\n");
    persistentBuffer.Reset();

    XDestroyImage(image);
    image = nullptr;
}

uint32_t XScreenshooter::ByteLength() const {
    return attributes.width*attributes.height*3;
}

uint8_t* XScreenshooter::Data() const
{
    return data;
}

void XScreenshooter::GetImage() {
    if (attributes.map_state != IsViewable)
    {
        throw std::runtime_error("Window map state must be viewable for screenshot taking");
    }
    image = XGetImage(display, window, attributes.x, attributes.y, attributes.width, attributes.height, AllPlanes, XYPixmap);
    if(!image) {
        throw std::runtime_error("Failed to get image");
    }
    uint32_t x, y;
    uint32_t offset;
    uint32_t row_length = image->width * 3;
    uint64_t pixel;
    uint8_t red, blue, green;
    for(y = 0; y < image->height; y++) {
        for(x = 0; x < image->width; x++) {
            pixel = XGetPixel(image, x, y);
            red = (pixel & image->red_mask) >> 16;
            green = (pixel & image->green_mask) >> 8;
            blue = (pixel & image->blue_mask);
            offset = (y*row_length)+(x*3);
            data[offset] = red;
            data[offset+1] = green;
            data[offset+2] = blue;
        }
    }
    XDestroyImage(image);
    image = nullptr;
}

void XScreenshooter::Init(const v8::Local<v8::Object> exports) {
    std::map<std::string, Nan::FunctionCallback> methods {
        { "constructor", Constructor },
        { "getImage", GetImage }
    };

    Nan::Set(
        exports,
        Nan::New<v8::String>("Screenshooter").ToLocalChecked(),
        ClassCreator::NewClass<XScreenshooter>("Screenshooter", methods)
    );
}

v8::Local<v8::Object> XScreenshooter::GetArrayBuffer() {
    if(!persistentBuffer.IsEmpty()) {
        return Nan::New(persistentBuffer);
    }

    const auto buffer = CreateExternalArrayBuffer(
        reinterpret_cast<char*>(data),
        ByteLength()
    );

    persistentBuffer.Reset(buffer);

    return New(persistentBuffer);
}

NAN_METHOD(XScreenshooter::Constructor) {
    if(!info[0]->IsObject()) {
        Nan::ThrowError("First argument must be a valid XDO instance");
        return;
    }

    const auto tool = Unwrap<XdoTool>(Nan::To<v8::Object>(info[0]).ToLocalChecked());

    Window window {};
    if(!TypeConverter::GetWindow(info[1],window)) {
        window = XRootWindow(tool->GetXdo()->xdpy, 0);
        if(!window) {
            Nan::ThrowError("Could not get root window");
            return;
        }
    }

    const auto ss = new XScreenshooter(tool->GetXdo()->xdpy, window);
    ss->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(XScreenshooter::GetImage) {
    const auto ss = Unwrap<XScreenshooter>(info.This());
    const auto task = new XdoToolTask_GetImage(ss);
    auto* callback = new Nan::Callback(Nan::To<v8::Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}
