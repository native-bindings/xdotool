#include "Screenshooter.h"
#include "XTypeConverter.h"
#include "XdoToolTaskWorker.h"
#include "tasks/GetImage.h"
#include "XdoTool.h"
#include "ClassCreator.h"

#include <X11/Xutil.h>

XScreenshooter::XScreenshooter(Display* display, Window window): XService(display), window(window) {
    if(XGetWindowAttributes(display, window, &attributes) != X_OK) {
        throw std::runtime_error("Failed to get window attributes");
    }
    data = (uint8_t*) malloc(attributes.width*attributes.height*3);
    if(data == nullptr) {
        throw std::runtime_error("Memory allocation failed");
    }
    AddResource(data);
}

XScreenshooter::~XScreenshooter() {
    arrayBuffer.Reset();
}

uint32_t XScreenshooter::ByteLength() {
    return attributes.width*attributes.height*3;
}

uint8_t* XScreenshooter::Data() {
    return data;
}

void XScreenshooter::GetImage() {
    image = XGetImage(display, window, attributes.x, attributes.y, attributes.width, attributes.height, AllPlanes, ZPixmap);
    if(!image) {
        throw std::runtime_error("Failed to get image");
    }
    uint32_t x, y;
    uint32_t offset;
    uint32_t row_length = image->width*3;
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
}

using v8::Local;
using v8::String;
using v8::Object;
using v8::Function;
using v8::FunctionTemplate;
using Nan::SetPrototypeMethod;
using Nan::FunctionCallback;
using Nan::New;
using Nan::Callback;
using Nan::Set;
using Nan::To;

Persistent<Function> XScreenshooter::constructor;

void XScreenshooter::Init(Local<Object> exports) {
    std::map<std::string, FunctionCallback> methods {
        { "constructor", Constructor },
        { "getImage", GetImage }
    };

    Set(
        exports,
        New<String>("Screenshooter").ToLocalChecked(),
        ClassCreator::NewClass<XScreenshooter>("Screenshooter", methods)
    );
}

Local<ArrayBuffer> XScreenshooter::GetArrayBuffer() {
    if(!arrayBuffer.IsEmpty()) {
        return New(arrayBuffer);
    }

    Local<ArrayBuffer> buffer = ArrayBuffer::New(
        Nan::GetCurrentContext()->GetIsolate(),
        data,
        ByteLength(),
        v8::ArrayBufferCreationMode::kExternalized
    );
    arrayBuffer.Reset(buffer);
    return New(arrayBuffer);
}

NAN_METHOD(XScreenshooter::Constructor) {
    if(!info[0]->IsObject()) {
        Nan::ThrowError("First argument must be a valid XDO instance");
        return;
    }

    auto tool = Nan::ObjectWrap::Unwrap<XdoTool>(Nan::To<Object>(info[0]).ToLocalChecked());

    Window window = XTypeConverter::GetWindow(info[1]);
    if(!window) {
        window = XRootWindow(tool->GetXdo()->xdpy, 0);
        if(!window) {
            Nan::ThrowError("Could not get root window");
            return;
        }
    }

    auto ss = new XScreenshooter(tool->GetXdo()->xdpy, window);
    ss->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(XScreenshooter::GetImage) {
    auto ss = Nan::ObjectWrap::Unwrap<XScreenshooter>(info.This());
    auto task = new XdoToolTask_GetImage(ss);
    Callback* callback = new Callback(To<Function>(info[0]).ToLocalChecked());
    AsyncQueueWorker(new XdoToolTaskWorker(callback, task));
}
