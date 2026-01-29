#ifndef NODE_XDOTOOL_RESOURCE_MANAGER_H_
#define NODE_XDOTOOL_RESOURCE_MANAGER_H_

#include <nan.h>
#include <vector>

class ResourceManager {
public:
    void AddResource(void* ptr);
    virtual ~ResourceManager();
private:
    std::vector<void*> pointers;
};

v8::Local<v8::Object> CreateExternalArrayBuffer(char* nativeData, std::uint32_t len);

#endif // NODE_XDOTOOL_RESOURCE_MANAGER_H_
