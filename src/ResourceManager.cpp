#include "ResourceManager.h"

void ResourceManager::AddResource(void* ptr) {
    pointers.emplace_back(ptr);
}

ResourceManager::~ResourceManager() {
    for(auto & p : pointers) {
        free(p);
    }
}