#ifndef NODE_XDOTOOL_RESOURCE_MANAGER_H_
#define NODE_XDOTOOL_RESOURCE_MANAGER_H_

#include <vector>
#include <string>

class ResourceManager {
public:
    void AddResource(void* ptr);
    ~ResourceManager();
private:
    std::vector<void*> pointers;
};

#endif // NODE_XDOTOOL_RESOURCE_MANAGER_H_
